#include "OnePole.h"

const std::size_t dsp::OnePole::FREQUENCY = 1;

dsp::OnePole::OnePole() : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR), mode(Mode::LOW_PASS) {
    pushInput(Connection::Type::HERTZ);
}

void dsp::OnePole::setNumChannels(std::size_t numChannels) {
    lock();
    setNumChannelsNoLock(numChannels);
    y1.resize(numChannels, 0.0);
    unlock();
}

dsp::OnePole::Mode dsp::OnePole::getMode() {
    return mode;
}

void dsp::OnePole::setMode(Mode mode) {
    this->mode = mode;
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::OnePole::getFrequency() {
    return getInput(FREQUENCY);
}

void dsp::OnePole::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            DSP_FLOAT a0, b1;
            switch (mode) {
                case Mode::LOW_PASS:
                    b1 = exp(-TAU * frequencyBuffer[k] * getOneOverSampleRate());
                    a0 = 1.0 - b1;
                    break;
                case Mode::HIGH_PASS:
                    b1 = -exp(-TAU * (0.5 - frequencyBuffer[k] * getOneOverSampleRate()));
                    a0 = 1.0 + b1;
                    break;
            }
            y1[i] = outputBuffer[k] = a0 * inputBuffer[k] + b1 * y1[i];
        }
    }
}
