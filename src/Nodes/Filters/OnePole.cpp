#include "OnePole.h"

dsp::OnePole::OnePole(Type type)
        : Transformer(type, type == Type::INTEGER ? Type::LOGARITHMIC : type)
        , mode(Mode::LOW_PASS)
        , frequency(std::make_shared<Input>(Type::HERTZ)) {
    getInputs().push_back(frequency);
}

dsp::OnePole::Mode dsp::OnePole::getMode() const {
    return mode;
}

void dsp::OnePole::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::Input> dsp::OnePole::getFrequency() const {
    return frequency;
}

void dsp::OnePole::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    state.resize(numChannels, 0.0);
}

void dsp::OnePole::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *frequencyChannel = getFrequency()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample radians = PI * frequencyChannel[sample] * getOneOverSampleRate();
            Sample delta = tan(radians / (1.0 + radians)) * (inputChannel[sample] - state[channel]);
            state[channel] += delta;
            switch (mode) {
                case Mode::LOW_PASS: outputChannel[sample] = state[channel]; break;
                case Mode::HIGH_PASS: outputChannel[sample] = inputChannel[sample] - state[channel]; break;
            }
            state[channel] += delta;
        }
    }
}
