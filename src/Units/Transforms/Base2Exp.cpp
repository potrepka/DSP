#include "Base2Exp.h"

dsp::Base2Exp::Base2Exp(Space space)
        : Processor(Type::LINEAR, Type::RATIO, space) {}

void dsp::Base2Exp::process() {
    Unit::process();
    transform([](Sample x) { return exp2(x); });
}
