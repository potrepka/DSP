#include "Phasor.h"

dsp::Phasor::Phasor()
        : Producer(Type::RATIO)
        , frequency(std::make_shared<Input>(Type::HERTZ))
        , mode(std::make_shared<Input>(Type::INTEGER, Space::TIME, Mode::MAX))
        , reset(std::make_shared<Input>(Type::BOOLEAN)) {
    getInputs().push_back(frequency);
    getInputs().push_back(mode);
    getInputs().push_back(reset);
}

std::shared_ptr<dsp::Input> dsp::Phasor::getFrequency() const {
    return frequency;
}

std::shared_ptr<dsp::Input> dsp::Phasor::getMode() const {
    return mode;
}

std::shared_ptr<dsp::Input> dsp::Phasor::getReset() const {
    return reset;
}

void dsp::Phasor::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    phase.resize(numChannels, 0.0);
}

void dsp::Phasor::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *frequencyChannel = getFrequency()->getWrapper().getChannelPointer(channel);
        Sample *modeChannel = getMode()->getWrapper().getChannelPointer(channel);
        Sample *resetChannel = getReset()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (resetChannel[sample]) {
                phase[channel] = 0.0;
            } else if (modeChannel[sample] == Mode::WRAPPED) {
                phase[channel] = wrap(phase[channel], 1.0);
            }
            outputChannel[sample] = phase[channel];
            phase[channel] += frequencyChannel[sample] * getOneOverSampleRate();
        }
    }
}
