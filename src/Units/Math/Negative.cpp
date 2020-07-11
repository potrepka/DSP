#include "Negative.h"

dsp::Negative::Negative(Type type, Space space) : Processor(type, type, space) {
    assert(type != Type::BINARY);
}

void dsp::Negative::process() {
    Unit::process();
    transform(negative);
}
