#include "Base2Log.h"

dsp::Base2Log::Base2Log(Space space)
        : Processor(Type::RATIO, Type::LINEAR, space) {}

void dsp::Base2Log::process() {
    Unit::process();
    transform([](Sample x) { return log2(x); });
}
