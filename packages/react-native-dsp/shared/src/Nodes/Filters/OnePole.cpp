#include "OnePole.h"

dsp::OnePole::OnePole(Type type)
    : Transformer(type, type),
      frequency(std::make_shared<Input>(Type::HERTZ)),
      mode(std::make_shared<Input>(Type::INTEGER, Space::TIME, Mode::MAX)) {
  getInputs().push_back(frequency);
  getInputs().push_back(mode);
}

std::shared_ptr<dsp::Input> dsp::OnePole::getFrequency() const {
  return frequency;
}

std::shared_ptr<dsp::Input> dsp::OnePole::getMode() const { return mode; }

dsp::FrequencyResponse dsp::OnePole::getFrequencyResponse(size_t channel,
                                                          Sample frequency) {
  lock();
  DSP_ASSERT(channel < getNumChannels());
  if (getNumSamples() > 0) {
    const size_t lastSample = getNumSamples() - 1;
    const Sample sampleRate = getSampleRate();
    const Sample oneOverSampleRate = getOneOverSampleRate();
    const Sample f =
        getFrequency()->getWrapper().getSample(channel, lastSample);
    unlock();
    const Sample radians =
        PI * clip(f, 0.0, 0.5 * sampleRate) * oneOverSampleRate;
    const Sample g = tan(radians / (1.0 + radians));
    const Sample omega = 2.0 * PI * frequency * oneOverSampleRate;
    const Sample cosW = cos(omega);
    const Sample sinW = sin(omega);
    const Sample a = g * (1.0 + cosW);
    const Sample b = -g * sinW;
    const Sample gPlusOne = g + 1.0;
    const Sample gMinusOne = g - 1.0;
    const Sample c = gPlusOne + gMinusOne * cosW;
    const Sample d = -gMinusOne * sinW;
    const Sample magnitudeNum = a * a + b * b;
    const Sample magnitudeDen = c * c + d * d;
    const Sample magnitude = sqrt(magnitudeNum / magnitudeDen);
    const Sample phaseNum = b * c - a * d;
    const Sample phaseDen = a * c + b * d;
    const Sample bipolar = ONE_OVER_TAU * atan2(phaseNum, phaseDen);
    const Sample phase = bipolar < 0.0 ? bipolar + 1.0 : bipolar;
    return {magnitude, phase};
  } else {
    unlock();
    return {1.0, 0.0};
  }
}

void dsp::OnePole::setNumOutputChannelsNoLock(size_t numChannels) {
  Node::setNumOutputChannelsNoLock(numChannels);
  state.resize(numChannels, 0.0);
}

void dsp::OnePole::processNoLock() {
  for (size_t channel = 0; channel < getNumChannels(); ++channel) {
    Sample* inputChannel = getInput()->getWrapper().getChannelPointer(channel);
    Sample* frequencyChannel =
        getFrequency()->getWrapper().getChannelPointer(channel);
    Sample* modeChannel = getMode()->getWrapper().getChannelPointer(channel);
    Sample* outputChannel =
        getOutput()->getWrapper().getChannelPointer(channel);
    for (size_t sample = 0; sample < getNumSamples(); ++sample) {
      Sample& input = inputChannel[sample];
      Sample& frequency = frequencyChannel[sample];
      Sample& mode = modeChannel[sample];
      Sample& output = outputChannel[sample];
      if (isnan(state[channel])) {
        state[channel] = 0.0;
      }
      const Sample sampleRate = getSampleRate();
      const Sample oneOverSampleRate = getOneOverSampleRate();
      const Sample radians =
          PI * clip(frequency, 0.0, 0.5 * sampleRate) * oneOverSampleRate;
      const Sample delta =
          tan(radians / (1.0 + radians)) * (input - state[channel]);
      state[channel] += delta;
      switch (static_cast<int>(mode)) {
        case Mode::LOW_PASS:
          output = state[channel];
          break;
        case Mode::HIGH_PASS:
          output = input - state[channel];
          break;
      }
      state[channel] += delta;
    }
  }
}
