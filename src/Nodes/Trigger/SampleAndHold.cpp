#include "SampleAndHold.h"

dsp::SampleAndHold::SampleAndHold(Type type)
        : Transformer(type, type)
        , trigger(std::make_shared<Input>(Type::INTEGER)) {
    getInputs().push_back(trigger);
}

std::shared_ptr<dsp::Input> dsp::SampleAndHold::getTrigger() const {
    return trigger;
}

void dsp::SampleAndHold::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    memory.resize(numChannels, std::numeric_limits<Sample>::quiet_NaN());
}

void dsp::SampleAndHold::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *triggerChannel = getTrigger()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (triggerChannel[sample]) {
                memory[channel] = inputChannel[sample];
            }
            outputChannel[sample] =
                    std::isnan(memory[channel]) ? getOutput()->getChannelValues()[channel] : memory[channel];
        }
    }
}
