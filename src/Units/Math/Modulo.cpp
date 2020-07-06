#include "Modulo.h"

const unsigned int dsp::Modulo::DIVISOR = 1;

dsp::Modulo::Modulo(Connection::Type type, Connection::Space space) : Processor(type, type, space) {
    assert(type != Connection::Type::BINARY);
    assert(type != Connection::Type::INTEGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Modulo::getDivisor() {
    return getInput(DIVISOR);
}

void dsp::Modulo::process() {
    Unit::process();
    transform(getDivisor(), [](DSP_FLOAT x, DSP_FLOAT y) { return y == 0.0 ? 0.0 : x - floor(x / y) * y; });
}
