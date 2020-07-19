#include "Phasor.h"

dsp::Phasor::Phasor()
        : Generator(Type::UNIPOLAR)
        , resetTrigger(pushInput(Type::BINARY))
        , frequency(pushInput(Type::HERTZ)) {}

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
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<Sample> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        std::vector<Sample> &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        std::vector<Sample> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (resetTriggerBuffer[k]) {
                phase[i] = 0.0;
            }
            outputBuffer[k] = phase[i];
            phase[i] = wrap(phase[i] + frequencyBuffer[k] * getOneOverSampleRate(), 0.0, 1.0);
        }
    }
}
