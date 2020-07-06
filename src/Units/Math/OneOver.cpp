#include "OneOver.h"

dsp::OneOver::OneOver(Connection::Space space) : Processor(Connection::Type::RATIO, Connection::Type::RATIO, space) {}

void dsp::OneOver::process() {
    Unit::process();
    transform([](DSP_FLOAT x) { return 1.0 / x; });
}
