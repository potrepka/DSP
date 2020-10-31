#include "OnePole.h"

dsp::OnePole::OnePole(Type type)
        : Transformer(type, type)
        , frequency(std::make_shared<Input>(Type::HERTZ))
        , mode(std::make_shared<Input>(Type::INTEGER, Space::TIME, Mode::MAX)) {
    getInputs().push_back(frequency);
    getInputs().push_back(mode);
}

std::shared_ptr<dsp::Input> dsp::OnePole::getFrequency() const {
    return frequency;
}

std::shared_ptr<dsp::Input> dsp::OnePole::getMode() const {
    return mode;
}

void dsp::OnePole::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    state.resize(numChannels, 0.0);
}

void dsp::OnePole::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *frequencyChannel = getFrequency()->getWrapper().getChannelPointer(channel);
        Sample *modeChannel = getMode()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample &input = inputChannel[sample];
            Sample &frequency = frequencyChannel[sample];
            Sample &mode = modeChannel[sample];
            Sample &output = outputChannel[sample];
            if (isnan(state[channel])) {
                state[channel] = 0.0;
            }
            const Sample radians = PI * frequency * getOneOverSampleRate();
            const Sample delta = tan(radians / (1.0 + radians)) * (input - state[channel]);
            state[channel] += delta;
            switch (static_cast<int>(mode)) {
                case Mode::LOW_PASS: output = state[channel]; break;
                case Mode::HIGH_PASS: output = input - state[channel]; break;
            }
            state[channel] += delta;
        }
    }
}
