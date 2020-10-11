#include "BeatTrigger.h"

dsp::BeatTrigger::BeatTrigger()
        : Producer(Type::BOOLEAN)
        , intervalTime(std::make_shared<Input>(Type::SECONDS))
        , delayTime(std::make_shared<Input>(Type::SECONDS))
        , reset(std::make_shared<Input>(Type::BOOLEAN))
        , currentTime(std::make_shared<Output>(Type::SECONDS)) {
    getInputs().push_back(intervalTime);
    getInputs().push_back(delayTime);
    getInputs().push_back(reset);
    getOutputs().push_back(currentTime);
}

std::shared_ptr<dsp::Input> dsp::BeatTrigger::getIntervalTime() const {
    return intervalTime;
}

std::shared_ptr<dsp::Input> dsp::BeatTrigger::getDelayTime() const {
    return delayTime;
}

std::shared_ptr<dsp::Input> dsp::BeatTrigger::getReset() const {
    return reset;
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
        Sample *intervalTimeChannel = getIntervalTime()->getWrapper().getChannelPointer(channel);
        Sample *delayTimeChannel = getDelayTime()->getWrapper().getChannelPointer(channel);
        Sample *resetChannel = getReset()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample *currentTimeChannel = getCurrentTime()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (resetChannel[sample]) {
                index[channel] = 0.0;
            }
            Sample interval = abs(intervalTimeChannel[sample] * getSampleRate());
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
