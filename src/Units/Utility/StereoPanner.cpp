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
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        Array &directionBuffer = getDirection()->getChannel(i)->getBuffer();
        Array &leftBuffer = getLeft()->getChannel(i)->getBuffer();
        Array &rightBuffer = getRight()->getChannel(i)->getBuffer();
        Iterator inputIterator = inputBuffer.begin();
        Iterator directionIterator = directionBuffer.begin();
        Iterator leftIterator = leftBuffer.begin();
        Iterator rightIterator = rightBuffer.begin();
        while (inputIterator != inputBuffer.end()) {
#if DSP_USE_VC
            Vector left;
            Vector right;
#else
            Sample left;
            Sample right;
#endif
            switch (mode) {
                case Mode::CONSTANT_POWER:
#if DSP_USE_VC
                    left = Vc::cos(PI_OVER_TWO * bipolarToUnipolar(*directionIterator));
                    right = Vc::cos(PI_OVER_TWO * bipolarToUnipolar(negative(*directionIterator)));
#else
                    left = cos(PI_OVER_TWO * bipolarToUnipolar(*directionIterator));
                    right = cos(PI_OVER_TWO * bipolarToUnipolar(negative(*directionIterator)));
#endif
                    break;
                case Mode::LINEAR:
                    left = bipolarToUnipolar(negative(*directionIterator));
                    right = bipolarToUnipolar(*directionIterator);
                    break;
            }
            *leftIterator = left * *inputIterator;
            *rightIterator = right * *inputIterator;

            ++inputIterator;
            ++directionIterator;
            ++leftIterator;
            ++rightIterator;
        }
    }
}
