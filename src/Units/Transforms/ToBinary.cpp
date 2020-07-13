#include "ToBinary.h"

dsp::ToBinary::ToBinary(Type type, Space space)
        : Processor(type, Type::BINARY, space) {}

void dsp::ToBinary::process() {
    Unit::process();
    transform(toBinary);
}
