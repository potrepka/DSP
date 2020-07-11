#include "HertzToSeconds.h"

dsp::HertzToSeconds::HertzToSeconds() : Processor(Type::HERTZ, Type::SECONDS) {}

void dsp::HertzToSeconds::process() {
    Unit::process();
    transform(oneOver);
}
