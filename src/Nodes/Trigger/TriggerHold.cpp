#include "TriggerHold.h"

dsp::TriggerHold::TriggerHold()
        : Transformer(Type::BOOLEAN, Type::BOOLEAN)
        , holdTime(std::make_shared<Input>(Type::SECONDS))
        , currentTime(std::make_shared<Output>(Type::SECONDS)) {
    getInputs().push_back(holdTime);
    getOutputs().push_back(currentTime);
}

std::shared_ptr<dsp::Input> dsp::TriggerHold::getHoldTime() const {
    return holdTime;
}

std::shared_ptr<dsp::Output> dsp::TriggerHold::getCurrentTime() const {
    return currentTime;
}

void dsp::TriggerHold::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    index.resize(numChannels, 0.0);
}

void dsp::TriggerHold::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *holdTimeChannel = getHoldTime()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample *currentTimeChannel = getCurrentTime()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (inputChannel[sample]) {
                index[channel] = 0.0;
            }
            const Sample holdTime = holdTimeChannel[sample] * getSampleRate();
            outputChannel[sample] = index[channel] < holdTime;
            currentTimeChannel[sample] = index[channel] * getOneOverSampleRate();
            index[channel] += 1.0;
        }
    }
}
