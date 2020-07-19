#include "AbsoluteValue.h"

dsp::AbsoluteValue::AbsoluteValue(Type type, Space space)
        : Processor(type, type, space) {}

void dsp::AbsoluteValue::process() {
    Unit::process();
    transform([](Sample x) { return abs(x); });
}
