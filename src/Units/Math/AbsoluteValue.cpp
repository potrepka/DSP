#include "AbsoluteValue.h"

dsp::AbsoluteValue::AbsoluteValue(Type type, Space space) : Processor(type, type, space) {}

void dsp::AbsoluteValue::process() {
    Unit::process();
    transform([](DSP_FLOAT x) { return abs(x); });
}
