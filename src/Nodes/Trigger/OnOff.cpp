#include "OnOff.h"

dsp::OnOff::OnOff()
        : Producer(Type::INTEGER)
        , onTrigger(std::make_shared<Input>(Type::INTEGER))
        , offTrigger(std::make_shared<Input>(Type::INTEGER)) {
    getInputs().push_back(onTrigger);
    getInputs().push_back(offTrigger);
}

std::shared_ptr<dsp::Input> dsp::OnOff::getOnTrigger() const {
    return onTrigger;
}

std::shared_ptr<dsp::Input> dsp::OnOff::getOffTrigger() const {
    return offTrigger;
}

void dsp::OnOff::setNumOutputChannelsNoLock(int numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    state.resize(numChannels, false);
}

void dsp::OnOff::processNoLock() {
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *onTriggerChannel = getOnTrigger()->getBlock().getChannelPointer(channel);
        Sample *offTriggerChannel = getOffTrigger()->getBlock().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getBlock().getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            if (onTriggerChannel[sample]) {
                state[channel] = true;
            }
            if (offTriggerChannel[sample]) {
                state[channel] = false;
            }
            outputChannel[sample] = state[channel];
        }
    }
}
