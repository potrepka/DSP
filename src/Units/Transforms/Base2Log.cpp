#include "Base2Log.h"

dsp::Base2Log::Base2Log() : Processor(Type::RATIO, Type::LINEAR) {}

void dsp::Base2Log::process() {
    Unit::process();
    transform([](DSP_FLOAT x) { return log2(x); });
}
