#include "ToBinary.h"

dsp::ToBinary::ToBinary(Type type) : Processor(type, Type::BINARY) {}

void dsp::ToBinary::process() {
    Unit::process();
    transform(toBinary);
}
