#include "BeatTrigger.h"

dsp::BeatTrigger::BeatTrigger()
        : Producer(Type::BINARY)
        , resetTrigger(std::make_shared<Input>(Type::BINARY))
        , intervalDuration(std::make_shared<Input>(Type::SECONDS))
        , delayTime(std::make_shared<Input>(Type::SECONDS))
        , currentTime(std::make_shared<Output>(Type::SECONDS)) {
    getInputs().push_back(resetTrigger);
    getInputs().push_back(intervalDuration);
    getInputs().push_back(delayTime);
    getOutputs().push_back(currentTime);
}

std::shared_ptr<dsp::Input> dsp::BeatTrigger::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::Input> dsp::BeatTrigger::getIntervalDuration() const {
    return intervalDuration;
}

std::shared_ptr<dsp::Input> dsp::BeatTrigger::getDelayTime() const {
    return delayTime;
}

std::shared_ptr<dsp::Output> dsp::BeatTrigger::getCurrentTime() const {
    return currentTime;
}

void dsp::BeatTrigger::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    index.resize(numChannels, 0.0);
}

void dsp::BeatTrigger::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *resetTriggerChannel = getResetTrigger()->getWrapper().getChannelPointer(channel);
        Sample *intervalDurationChannel = getIntervalDuration()->getWrapper().getChannelPointer(channel);
        Sample *delayTimeChannel = getDelayTime()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample *currentTimeChannel = getCurrentTime()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (resetTriggerChannel[sample] || intervalDurationChannel[sample] == 0.0) {
                index[channel] = 0.0;
            } else {
                Sample interval = abs(intervalDurationChannel[sample] * getSampleRate());
                Sample delayed = index[channel] - delayTimeChannel[sample] * getSampleRate();
                if (delayed >= interval) {
                    index[channel] -= floor(delayed / interval) * interval;
                }
            }
            outputChannel[sample] = index[channel] < 1.0 ? 1.0 : 0.0;
            currentTimeChannel[sample] = index[channel] * getOneOverSampleRate();
            index[channel] += 1.0;
        }
    }
}
