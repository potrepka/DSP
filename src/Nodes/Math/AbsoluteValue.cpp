#include "AbsoluteValue.h"

dsp::AbsoluteValue::AbsoluteValue(Type type, Space space)
        : Transformer(type, type, space) {}

void dsp::AbsoluteValue::processNoLock() {
    getOutput()->getWrapper().replaceWithAbsoluteValueOf(getInput()->getWrapper());
}
