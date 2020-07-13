#include "Modulo.h"

dsp::Modulo::Modulo(Type type, Space space)
        : Processor(type, type, space)
        , DIVISOR(pushInput(type, space, 1.0)) {
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
