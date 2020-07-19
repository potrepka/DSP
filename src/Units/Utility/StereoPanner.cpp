#include "StereoPanner.h"

dsp::StereoPanner::StereoPanner(Type type, Space space)
        : Consumer(type, space)
        , direction(pushInput(Type::BIPOLAR, space))
        , left(pushOutput(type, space))
        , right(pushOutput(type, space))
        , mode(Mode::CONSTANT_POWER) {}

dsp::StereoPanner::Mode dsp::StereoPanner::getMode() const {
    return mode;
}

void dsp::StereoPanner::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::StereoPanner::getDirection() const {
    return direction;
}

std::shared_ptr<dsp::OutputParameter> dsp::StereoPanner::getLeft() const {
    return left;
}

std::shared_ptr<dsp::OutputParameter> dsp::StereoPanner::getRight() const {
    return right;
}

void dsp::StereoPanner::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        Array &directionBuffer = getDirection()->getChannel(i)->getBuffer();
        Array &leftBuffer = getLeft()->getChannel(i)->getBuffer();
        Array &rightBuffer = getRight()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            Sample left;
            Sample right;
            switch (mode) {
                case Mode::CONSTANT_POWER:
                    left = cos(PI_OVER_TWO * bipolarToUnipolar(directionBuffer[k]));
                    right = cos(PI_OVER_TWO * bipolarToUnipolar(-directionBuffer[k]));
                    break;
                case Mode::LINEAR:
                    left = bipolarToUnipolar(-directionBuffer[k]);
                    right = bipolarToUnipolar(directionBuffer[k]);
                    break;
            }
            leftBuffer[k] = left * inputBuffer[k];
            rightBuffer[k] = right * inputBuffer[k];
        }
    }
}
