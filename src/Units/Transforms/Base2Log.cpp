#include "Base2Log.h"

dsp::Base2Log::Base2Log(Space space) : Processor(Type::RATIO, Type::LINEAR, space) {}

void dsp::Base2Log::process() {
    Unit::process();
    transform([](DSP_FLOAT x) { return log2(x); });
}
