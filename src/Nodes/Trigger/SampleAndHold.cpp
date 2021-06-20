#include "SampleAndHold.h"

dsp::SampleAndHold::SampleAndHold(Type type)
        : Transformer(type, type)
        , gate(std::make_shared<Input>(Type::BOOLEAN))
        , reset(std::make_shared<Input>(Type::BOOLEAN)) {
    getInputs().push_back(gate);
    getInputs().push_back(reset);
}

std::shared_ptr<dsp::Input> dsp::SampleAndHold::getGate() const {
    return gate;
}

std::shared_ptr<dsp::Input> dsp::SampleAndHold::getReset() const {
    return reset;
}

void dsp::SampleAndHold::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    state.resize(numChannels, 0.0);
}

void dsp::SampleAndHold::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *gateChannel = getGate()->getWrapper().getChannelPointer(channel);
        Sample *resetChannel = getReset()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample channelValue = getOutput()->getChannelValue(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (resetChannel[sample]) {
                state[channel] = 0.0;
            }
            if (gateChannel[sample]) {
                state[channel] = inputChannel[sample];
            }
            outputChannel[sample] = state[channel];
        }
    }
}
