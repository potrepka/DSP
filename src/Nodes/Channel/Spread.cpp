#include "Spread.h"

dsp::Spread::Spread(Type type, Domain domain)
    : Transformer(type, domain),
      mode(Mode::UNIPOLAR),
      spread(std::make_shared<Input>(type, domain)) {
  getInputs().push_back(spread);
}

dsp::Spread::Mode dsp::Spread::getMode() const { return mode; }

void dsp::Spread::setMode(Mode mode) {
  lock();
  this->mode = mode;
  unlock();
}

std::shared_ptr<dsp::Input> dsp::Spread::getSpread() const { return spread; }

void dsp::Spread::processNoLock() {
  size_t numChannelsMinusOne = getNumChannels() - 1;
  Sample modeValue = mode == Mode::BIPOLAR ? 1.0 : 0.0;
  for (size_t channel = 0; channel < getNumChannels(); ++channel) {
    Sample* inputChannel = getInput()->getWrapper().getChannelPointer(channel);
    Sample* spreadChannel =
        getSpread()->getWrapper().getChannelPointer(channel);
    Sample* outputChannel =
        getOutput()->getWrapper().getChannelPointer(channel);
    Sample amount =
        numChannelsMinusOne > 0 ? channel / numChannelsMinusOne : 0.0;
    for (size_t sample = 0; sample < getNumSamples(); ++sample) {
      Sample& input = inputChannel[sample];
      Sample& spread = spreadChannel[sample];
      Sample& output = outputChannel[sample];
      output =
          input + spread * (numChannelsMinusOne > 0 ? amount - 0.5 * modeValue
                                                    : amount);
    }
  }
}
