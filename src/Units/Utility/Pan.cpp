#include "Pan.h"

const unsigned int dsp::Pan::DIRECTION = 0;

dsp::Pan::Pan(Space space) : Generator(Type::UNIPOLAR, space), mode(Mode::CONSTANT_POWER) {
    pushInput(Type::BIPOLAR, space);
}

dsp::Pan::Mode dsp::Pan::getMode() const {
    return mode;
}

void dsp::Pan::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Pan::getDirection() const {
    return getInput(DIRECTION);
}

void dsp::Pan::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &directionBuffer = getDirection()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            const DSP_FLOAT direction = (i % 2 == 0 ? 1 : -1) * directionBuffer[k];
            switch (mode) {
                case Mode::CONSTANT_POWER: outputBuffer[k] = cos(PI_OVER_TWO * bipolarToUnipolar(direction)); break;
                case Mode::LINEAR: outputBuffer[k] = bipolarToUnipolar(-direction); break;
            }
        }
    }
}
