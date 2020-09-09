#include "AbsoluteValue.h"

dsp::AbsoluteValue::AbsoluteValue(Space space)
        : Transformer(Type::RATIO, Type::RATIO, space) {}

void dsp::AbsoluteValue::processNoLock() {
    getOutput()->getBlock().replaceWithAbsoluteValueOf(getInput()->getBlock());
}
