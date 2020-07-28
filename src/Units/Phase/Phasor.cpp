#include "Phasor.h"

dsp::Phasor::Phasor()
        : Generator(Type::UNIPOLAR)
        , resetTrigger(pushInput(Type::BINARY))
        , frequency(pushInput(Type::HERTZ))
        , mode(Mode::WRAPPED) {}

dsp::Phasor::Mode dsp::Phasor::getMode() {
    return mode;
}

void dsp::Phasor::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::Phasor::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::InputParameter> dsp::Phasor::getFrequency() const {
    return frequency;
}

void dsp::Phasor::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    phase.resize(numChannels, 0.0);
}

void dsp::Phasor::process() {
    Unit::process();
    const bool wrapped = mode == Mode::WRAPPED;
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        Array &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); ++k) {
            if (resetTriggerBuffer[k]) {
                phase[i] = 0.0;
            }
            outputBuffer[k] = phase[i];
            phase[i] += frequencyBuffer[k] * getOneOverSampleRate();
            if (wrapped) {
                phase[i] = wrap(phase[i], 1.0);
            }
        }
    }
}
