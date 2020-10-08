#include "NotGate.h"

dsp::NotGate::NotGate(Space space)
        : Transformer(Type::BOOLEAN, Type::BOOLEAN, space) {}

void dsp::NotGate::processNoLock() {
    transform([](Sample x) { return !x; });
}
