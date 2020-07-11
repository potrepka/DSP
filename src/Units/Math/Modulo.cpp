#include "Modulo.h"

const unsigned int dsp::Modulo::DIVISOR = 1;

dsp::Modulo::Modulo(Type type, Space space) : Processor(type, type, space) {
    assert(type != Type::BINARY);
    assert(type != Type::INTEGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Modulo::getDivisor() const {
    return getInput(DIVISOR);
}

void dsp::Modulo::process() {
    Unit::process();
    transform(getDivisor(), [](DSP_FLOAT x, DSP_FLOAT y) { return y == 0.0 ? 0.0 : x - floor(x / y) * y; });
}
