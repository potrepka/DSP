#include "SampleAndHold.h"

dsp::SampleAndHold::SampleAndHold(Type type)
        : Transformer(type, type)
        , trigger(std::make_shared<Input>(Type::INTEGER)) {
    getInputs().push_back(trigger);
}

std::shared_ptr<dsp::Input> dsp::SampleAndHold::getTrigger() const {
    return trigger;
}

void dsp::SampleAndHold::setNumOutputChannelsNoLock(int numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    memory.resize(numChannels, std::numeric_limits<Sample>::quiet_NaN());
}

void dsp::SampleAndHold::processNoLock() {
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getBlock().getChannelPointer(channel);
        Sample *triggerChannel = getTrigger()->getBlock().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getBlock().getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            if (triggerChannel[sample]) {
                memory[channel] = inputChannel[sample];
            }
            outputChannel[sample] =
                    std::isnan(memory[channel]) ? getOutput()->getChannelValues()[channel] : memory[channel];
        }
    }
}
