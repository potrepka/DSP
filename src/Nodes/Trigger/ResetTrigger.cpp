#include "ResetTrigger.h"

dsp::ResetTrigger::ResetTrigger()
        : Producer(Type::BINARY) {}

void dsp::ResetTrigger::reset() {
    lock();
    for (size_t channel = 0; channel < getNumOutputChannels(); ++channel) {
        state[channel] = true;
    }
    unlock();
}

void dsp::ResetTrigger::reset(size_t channel) {
    lock();
    DSP_ASSERT(channel < getNumChannels());
    state[channel] = true;
    unlock();
}

void dsp::ResetTrigger::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    state.resize(numChannels, false);
}

void dsp::ResetTrigger::processNoLock() {
    if (getNumSamples() > 0) {
        for (size_t channel = 0; channel < getNumOutputChannels(); ++channel) {
            if (state[channel]) {
                getOutput()->getWrapper().setSample(channel, 0, 1.0);
                state[channel] = false;
            }
        }
    }
}
