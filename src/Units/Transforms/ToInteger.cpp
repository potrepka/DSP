#include "ToInteger.h"

dsp::ToInteger::ToInteger(Connection::Type type) : Processor(type, Connection::Type::INTEGER) {}

void dsp::ToInteger::process() {
    Unit::process();
    transform(toInteger);
}
