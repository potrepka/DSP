#include "Base2Exp.h"

dsp::Base2Exp::Base2Exp() : Processor(Connection::Type::LINEAR, Connection::Type::RATIO) {}

void dsp::Base2Exp::process() {
    Unit::process();
    transform([](DSP_FLOAT x) { return exp2(x); });
}
