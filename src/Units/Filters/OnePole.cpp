#include "OnePole.h"

const unsigned int dsp::OnePole::FREQUENCY = 1;

dsp::OnePole::OnePole() : Processor(Type::BIPOLAR, Type::BIPOLAR), mode(Mode::LOW_PASS) {
    pushInput(Type::HERTZ);
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

void dsp::OnePole::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    y1.resize(numChannels, 0.0);
}

void dsp::OnePole::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            DSP_FLOAT radians = PI * frequencyBuffer[k] * getOneOverSampleRate();
            DSP_FLOAT delta = tan(radians / (1.0 + radians)) * (inputBuffer[k] - y1[i]);
            y1[i] += delta;
            switch (mode) {
                case Mode::LOW_PASS: outputBuffer[k] = y1[i]; break;
                case Mode::HIGH_PASS: outputBuffer[k] = inputBuffer[k] - y1[i]; break;
            }
            y1[i] += delta;
        }
    }
}
