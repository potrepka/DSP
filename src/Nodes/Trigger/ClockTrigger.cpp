#include "ClockTrigger.h"

dsp::ClockTrigger::ClockTrigger()
    : Producer(Type::BOOLEAN),
      interval(std::make_shared<Input>(Type::SECONDS)),
      delayTime(std::make_shared<Input>(Type::SECONDS)),
      reset(std::make_shared<Input>(Type::BOOLEAN)),
      currentTime(std::make_shared<Output>(Type::SECONDS)) {
  getInputs().push_back(interval);
  getInputs().push_back(delayTime);
  getInputs().push_back(reset);
  getOutputs().push_back(currentTime);
}

std::shared_ptr<dsp::Input> dsp::ClockTrigger::getInterval() const {
  return interval;
}

std::shared_ptr<dsp::Input> dsp::ClockTrigger::getDelayTime() const {
  return delayTime;
}

std::shared_ptr<dsp::Input> dsp::ClockTrigger::getReset() const {
  return reset;
}

std::shared_ptr<dsp::Output> dsp::ClockTrigger::getCurrentTime() const {
  return currentTime;
}

void dsp::ClockTrigger::setNumOutputChannelsNoLock(size_t numChannels) {
  Node::setNumOutputChannelsNoLock(numChannels);
  index.resize(numChannels, 0.0);
}

void dsp::ClockTrigger::processNoLock() {
  for (size_t channel = 0; channel < getNumChannels(); ++channel) {
    Sample* delayTimeChannel =
        getDelayTime()->getWrapper().getChannelPointer(channel);
    Sample* intervalChannel =
        getInterval()->getWrapper().getChannelPointer(channel);
    Sample* resetChannel = getReset()->getWrapper().getChannelPointer(channel);
    Sample* outputChannel =
        getOutput()->getWrapper().getChannelPointer(channel);
    Sample* currentTimeChannel =
        getCurrentTime()->getWrapper().getChannelPointer(channel);
    for (size_t sample = 0; sample < getNumSamples(); ++sample) {
      if (resetChannel[sample]) {
        index[channel] = 0.0;
      }
      const Sample interval = abs(intervalChannel[sample] * getSampleRate());
      const Sample delayTime = delayTimeChannel[sample] * getSampleRate();
      Sample delayed = index[channel] - delayTime;
      if (delayed >= interval) {
        if (interval > 0.0) {
          const Sample adjustment = floor(delayed / interval) * interval;
          index[channel] -= adjustment;
          delayed -= adjustment;
        } else {
          index[channel] = delayTime;
          delayed = 0.0;
        }
      }
      outputChannel[sample] = delayed >= 0.0 && delayed < 1.0;
      currentTimeChannel[sample] = delayed * getOneOverSampleRate();
      index[channel] += 1.0;
    }
  }
}
