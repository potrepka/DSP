#include "NotGate.h"

dsp::NotGate::NotGate(Space space)
        : Transformer(Type::INTEGER, Type::INTEGER, space) {}

void dsp::NotGate::processNoLock() {
    transform([](Sample x) { return !x; });
}
