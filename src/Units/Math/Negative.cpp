#include "Negative.h"

dsp::Negative::Negative(Connection::Type type, Connection::Space space) : Processor(type, type, space) {
    assert(type != Connection::Type::BINARY);
}

void dsp::Negative::process() {
    Unit::process();
    transform([](DSP_FLOAT x) { return -x; });
}
