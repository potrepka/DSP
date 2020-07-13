#include "StereoPanner.h"

const unsigned int dsp::StereoPanner::DIRECTION = 1;
const unsigned int dsp::StereoPanner::LEFT = 0;
const unsigned int dsp::StereoPanner::RIGHT = 1;

dsp::StereoPanner::StereoPanner(Type type, Space space) : Consumer(type, space), mode(Mode::CONSTANT_POWER) {
    pushInput(Type::BIPOLAR, space);
    pushOutput(type, space);
    pushOutput(type, space);
}

dsp::StereoPanner::Mode dsp::StereoPanner::getMode() const {
    return mode;
}

void dsp::StereoPanner::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::StereoPanner::getDirection() const {
    return getInput(DIRECTION);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::StereoPanner::getLeft() const {
    return getOutput(LEFT);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::StereoPanner::getRight() const {
    return getOutput(RIGHT);
}

void dsp::StereoPanner::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &directionBuffer = getDirection()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &leftBuffer = getLeft()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &rightBuffer = getRight()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            const DSP_FLOAT direction = (i % 2 == 0 ? 1 : -1) * directionBuffer[k];

            switch (mode) {
//                case Mode::CONSTANT_POWER: outputBuffer[k] = cos(PI_OVER_TWO * bipolarToUnipolar(direction)); break;
//                case Mode::LINEAR: outputBuffer[k] = bipolarToUnipolar(-direction); break;
            }
        }
    }
}
