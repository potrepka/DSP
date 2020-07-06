#include "Base2Log.h"

dsp::Base2Log::Base2Log() : Processor(Connection::Type::RATIO, Connection::Type::LINEAR) {}

void dsp::Base2Log::process() {
    Unit::process();
    transform([](DSP_FLOAT x) { return log2(x); });
}
