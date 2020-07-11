#include "Divide.h"

const unsigned int dsp::Divide::DIVISOR = 1;

dsp::Divide::Divide(Type type, Space space) : Processor(type, Type::RATIO, space) {
    pushInput(type, space);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Divide::getDivisor() const {
    return getInput(DIVISOR);
}

void dsp::Divide::process() {
    Unit::process();
    transform(getDivisor(), std::divides<DSP_FLOAT>());
}
