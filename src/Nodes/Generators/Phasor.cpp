#include "Phasor.h"

dsp::Phasor::Phasor()
        : Producer(Type::RATIO)
        , mode(Mode::UNBOUNDED)
        , resetTrigger(std::make_shared<Input>(Type::BINARY))
        , frequency(std::make_shared<Input>(Type::HERTZ)) {
    getInputs().push_back(resetTrigger);
    getInputs().push_back(frequency);
}

dsp::Phasor::Mode dsp::Phasor::getMode() const {
    return mode;
}

void dsp::Phasor::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::Input> dsp::Phasor::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::Input> dsp::Phasor::getFrequency() const {
    return frequency;
}

void dsp::Phasor::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    phase.resize(numChannels, 0.0);
}

void dsp::Phasor::processNoLock() {
    const bool wrapped = mode == Mode::WRAPPED;
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *resetTriggerChannel = getResetTrigger()->getWrapper().getChannelPointer(channel);
        Sample *frequencyChannel = getFrequency()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (resetTriggerChannel[sample]) {
                phase[channel] = 0.0;
            }
            outputChannel[sample] = phase[channel];
            phase[channel] += frequencyChannel[sample] * getOneOverSampleRate();
            if (wrapped) {
                phase[channel] = wrap(phase[channel], 1.0);
            }
        }
    }
}
