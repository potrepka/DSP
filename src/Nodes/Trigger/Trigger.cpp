#include "Trigger.h"

dsp::Trigger::Trigger()
        : Producer(Type::INTEGER)
        , resetTrigger(std::make_shared<Input>(Type::INTEGER))
        , intervalDuration(std::make_shared<Input>(Type::SECONDS))
        , delayTime(std::make_shared<Input>(Type::SECONDS))
        , currentTime(std::make_shared<Output>(Type::SECONDS)) {
    getInputs().push_back(resetTrigger);
    getInputs().push_back(intervalDuration);
    getInputs().push_back(delayTime);
    getOutputs().push_back(currentTime);
}

std::shared_ptr<dsp::Input> dsp::Trigger::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::Input> dsp::Trigger::getIntervalDuration() const {
    return intervalDuration;
}

std::shared_ptr<dsp::Input> dsp::Trigger::getDelayTime() const {
    return delayTime;
}

std::shared_ptr<dsp::Output> dsp::Trigger::getCurrentTime() const {
    return currentTime;
}

void dsp::Trigger::setNumOutputChannelsNoLock(int numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    index.resize(numChannels, 0.0);
}

void dsp::Trigger::processNoLock() {
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *resetTriggerChannel = getResetTrigger()->getBlock().getChannelPointer(channel);
        Sample *intervalDurationChannel = getIntervalDuration()->getBlock().getChannelPointer(channel);
        Sample *delayTimeChannel = getDelayTime()->getBlock().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getBlock().getChannelPointer(channel);
        Sample *currentTimeChannel = getCurrentTime()->getBlock().getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
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
