#include "OneOver.h"

dsp::OneOver::OneOver(Space space) : Processor(Type::RATIO, Type::RATIO, space) {}

void dsp::OneOver::process() {
    Unit::process();
    transform([](DSP_FLOAT x) { return 1.0 / x; });
}
