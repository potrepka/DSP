#include "Counter.h"

dsp::Counter::Counter()
        : Producer(Type::RATIO)
        , resetTrigger(std::make_shared<Input>(Type::INTEGER))
        , trigger(std::make_shared<Input>(Type::INTEGER))
        , speed(std::make_shared<Input>(Type::RATIO, Space::TIME, 1.0)) {
    getInputs().push_back(resetTrigger);
    getInputs().push_back(trigger);
    getInputs().push_back(speed);
}

std::shared_ptr<dsp::Input> dsp::Counter::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::Input> dsp::Counter::getTrigger() const {
    return trigger;
}

std::shared_ptr<dsp::Input> dsp::Counter::getSpeed() const {
    return speed;
}

void dsp::Counter::setNumOutputChannelsNoLock(int numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    memory.resize(numChannels, 0.0);
    index.resize(numChannels, 0.0);
}

void dsp::Counter::processNoLock() {
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *resetTriggerChannel = getResetTrigger()->getBlock().getChannelPointer(channel);
        Sample *triggerChannel = getTrigger()->getBlock().getChannelPointer(channel);
        Sample *speedChannel = getSpeed()->getBlock().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getBlock().getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            if (resetTriggerChannel[sample]) {
                index[channel] = 0.0;
            }
            if (triggerChannel[sample]) {
                memory[channel] = index[channel];
                index[channel] += speedChannel[sample];
            }
            outputChannel[sample] = memory[channel];
        }
    }
}
