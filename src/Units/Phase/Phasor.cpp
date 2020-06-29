#include "Phasor.h"

const std::size_t dsp::Phasor::FREQUENCY = 0;
const std::size_t dsp::Phasor::RESET_TRIGGER = 1;

dsp::Phasor::Phasor() : Generator(Connection::Type::UNIPOLAR) {
    pushInput(Connection::Type::HERTZ);
    pushInput(Connection::Type::INTEGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Phasor::getFrequency() {
    return getInput(FREQUENCY);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Phasor::getResetTrigger() {
    return getInput(RESET_TRIGGER);
}

void dsp::Phasor::setNumChannelsNoLock(std::size_t numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    phase.resize(numChannels, 0.0);
}

void dsp::Phasor::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (resetTriggerBuffer[k]) {
                phase[i] = 0.0;
            }
            outputBuffer[k] = phase[i];
            phase[i] = wrap(phase[i] + frequencyBuffer[k] * getOneOverSampleRate(), 0.0, 1.0);
        }
    }
}
