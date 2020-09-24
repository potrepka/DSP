#include "SampleAndHold.h"

dsp::SampleAndHold::SampleAndHold(Type type)
        : Transformer(type, type)
        , resetTrigger(std::make_shared<Input>(Type::BINARY))
        , gate(std::make_shared<Input>(Type::BINARY)) {
    getInputs().push_back(resetTrigger);
    getInputs().push_back(gate);
}

std::shared_ptr<dsp::Input> dsp::SampleAndHold::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::Input> dsp::SampleAndHold::getGate() const {
    return gate;
}

void dsp::SampleAndHold::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    state.resize(numChannels, std::numeric_limits<Sample>::quiet_NaN());
}

void dsp::SampleAndHold::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *resetTriggerChannel = getResetTrigger()->getWrapper().getChannelPointer(channel);
        Sample *gateChannel = getGate()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (resetTriggerChannel[sample]) {
                state[channel] = std::numeric_limits<Sample>::quiet_NaN();
            }
            if (gateChannel[sample]) {
                state[channel] = inputChannel[sample];
            }
            outputChannel[sample] =
                    std::isnan(state[channel]) ? getOutput()->getChannelValues()[channel] : state[channel];
        }
    }
}
