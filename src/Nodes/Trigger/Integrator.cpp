#include "Integrator.h"

dsp::Integrator::Integrator(Type type)
        : Transformer(type, type)
        , resetTrigger(std::make_shared<Input>(Type::BOOLEAN))
        , gate(std::make_shared<Input>(Type::BOOLEAN)) {
    getInputs().push_back(resetTrigger);
    getInputs().push_back(gate);
}

std::shared_ptr<dsp::Input> dsp::Integrator::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::Input> dsp::Integrator::getGate() const {
    return gate;
}

void dsp::Integrator::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    state.resize(numChannels, 0.0);
}

void dsp::Integrator::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *resetTriggerChannel = getResetTrigger()->getWrapper().getChannelPointer(channel);
        Sample *gateChannel = getGate()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (resetTriggerChannel[sample]) {
                state[channel] = 0.0;
            }
            if (gateChannel[sample]) {
                state[channel] += inputChannel[sample];
            }
            outputChannel[sample] = state[channel];
        }
    }
}
