#include "NotGate.h"

dsp::NotGate::NotGate(Space space)
        : Processor(Type::BINARY, Type::BINARY, space) {}

void dsp::NotGate::process() {
    Unit::process();
    transform([](Sample x) { return !x; });
}
