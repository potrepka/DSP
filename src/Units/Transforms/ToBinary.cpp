#include "ToBinary.h"

dsp::ToBinary::ToBinary(Connection::Type type) : Processor(type, Connection::Type::BINARY) {}

void dsp::ToBinary::process() {
    Unit::process();
    transform(toBinary);
}
