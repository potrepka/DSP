#include "ToInteger.h"

dsp::ToInteger::ToInteger(Type type, Space space)
        : Processor(type, Type::INTEGER, space) {}

void dsp::ToInteger::process() {
    Unit::process();
    transform(toInteger);
}
