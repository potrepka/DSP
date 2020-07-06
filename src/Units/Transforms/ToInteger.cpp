#include "ToInteger.h"

dsp::ToInteger::ToInteger(Type type) : Processor(type, Type::INTEGER) {}

void dsp::ToInteger::process() {
    Unit::process();
    transform(toInteger);
}
