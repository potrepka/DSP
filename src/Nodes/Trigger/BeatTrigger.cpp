#include "BeatTrigger.h"

dsp::BeatTrigger::BeatTrigger()
        : Producer(Type::BOOLEAN)
        , resetTrigger(std::make_shared<Input>(Type::BOOLEAN))
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
            if (resetTriggerChannel[sample]) {
                index[channel] = 0.0;
            }
            Sample interval = abs(intervalDurationChannel[sample] * getSampleRate());
            Sample delay = delayTimeChannel[sample] * getSampleRate();
            Sample adjusted = index[channel] - delay;
            if (adjusted >= interval) {
                if (interval) {
                    index[channel] -= floor(adjusted / interval) * interval;
                    adjusted = index[channel] - delay;
                } else {
                    index[channel] = delay;
                    adjusted = 0.0;
                }
            }
            outputChannel[sample] = adjusted < 1.0;
            currentTimeChannel[sample] = adjusted * getOneOverSampleRate();
            index[channel] += 1.0;
        }
    }
}
