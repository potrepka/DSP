#include "LogarithmicToRatio.h"

dsp::LogarithmicToRatio::LogarithmicToRatio(Space space)
        : Transformer(Type::LOGARITHMIC, Type::RATIO, space) {}

void dsp::LogarithmicToRatio::processNoLock() {
    getOutput()->getWrapper().copyFrom(getInput()->getWrapper());
}
