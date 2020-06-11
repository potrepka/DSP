#include "Phasor.h"

const std::size_t dsp::Phasor::FREQUENCY = 0;
const std::size_t dsp::Phasor::RESET_TRIGGER = 1;

dsp::Phasor::Phasor() : Generator(Connection::Type::UNIPOLAR) {
    pushInput(Connection::Type::HERTZ);
    pushInput(Connection::Type::BINARY);
}

void dsp::Phasor::setNumChannels(std::size_t numChannels) {
    lock();
    setNumChannelsNoLock(numChannels);
    phase.resize(numChannels, 0);
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Phasor::getFrequency() {
    return getInput(FREQUENCY);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Phasor::getResetTrigger() {
    return getInput(RESET_TRIGGER);
}

void dsp::Phasor::process() {
    Unit::process();
    for (int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        for (int k = 0; k < getBufferSize(); k++) {
            if (resetTriggerBuffer[k]) {
                phase[i] = 0;
            }
            outputBuffer[k] = phase[i];
            phase[i] = fmod(phase[i] + frequencyBuffer[k] / getSampleRate(), 1.0);
        }
    }
}
