#include "AbsoluteValue.h"

dsp::AbsoluteValue::AbsoluteValue(Type type, Space space)
        : Processor(type, type, space) {}

void dsp::AbsoluteValue::process() {
    Unit::process();
#if DSP_USE_VC
    transform([](Vector x) { return Vc::abs(x); });
#else
    transform([](Sample x) { return abs(x); });
#endif
}
