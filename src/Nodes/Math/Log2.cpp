#include "Log2.h"

dsp::Log2::Log2(Space space)
        : Transformer(Type::RATIO, Type::LOGARITHMIC, space) {}

void dsp::Log2::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf([](Sample x) { return log2(x); }, getInput()->getWrapper());
}
