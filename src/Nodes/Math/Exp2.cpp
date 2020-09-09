#include "Exp2.h"

dsp::Exp2::Exp2(Space space)
        : Transformer(Type::LOGARITHMIC, Type::RATIO, space) {}

void dsp::Exp2::processNoLock() {
    transform([](Sample x) { return exp2(x); });
}
