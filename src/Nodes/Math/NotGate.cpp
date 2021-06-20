#include "NotGate.h"

dsp::NotGate::NotGate(Space space)
        : Transformer(Type::BOOLEAN, Type::BOOLEAN, space) {}

void dsp::NotGate::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf([](Sample x) { return !x; }, getInput()->getWrapper());
}
