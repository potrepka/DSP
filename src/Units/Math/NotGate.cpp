#include "NotGate.h"

dsp::NotGate::NotGate(Space space)
        : Processor(Type::BINARY, Type::BINARY, space) {}

void dsp::NotGate::process() {
    Unit::process();
#if DSP_USE_VC
    transform([](Vector x) { return Vc::iif(!x, Vector::One(), Vector::Zero()); });
#else
    transform([](Sample x) { return !x; });
#endif
}
