#include "OnePole.h"

dsp::OnePole::OnePole(Type type)
        : Processor(type, type)
        , frequency(pushInput(Type::HERTZ))
        , mode(Mode::LOW_PASS) {}

dsp::OnePole::Mode dsp::OnePole::getMode() const {
    return mode;
}

void dsp::OnePole::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::OnePole::getFrequency() const {
    return frequency;
}

void dsp::OnePole::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    state.resize(numChannels, 0.0);
}

void dsp::OnePole::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        Array &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            Sample radians = PI * frequencyBuffer[k] * getOneOverSampleRate();
            Sample delta = tan(radians / (1.0 + radians)) * (inputBuffer[k] - state[i]);
            state[i] += delta;
            switch (mode) {
                case Mode::LOW_PASS: outputBuffer[k] = state[i]; break;
                case Mode::HIGH_PASS: outputBuffer[k] = inputBuffer[k] - state[i]; break;
            }
            state[i] += delta;
        }
    }
}
