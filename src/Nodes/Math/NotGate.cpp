#include "NotGate.h"

dsp::NotGate::NotGate(Space space)
        : Transformer(Type::BINARY, Type::BINARY, space) {}

void dsp::NotGate::processNoLock() {
    transform([](Sample x) { return !x; });
}
