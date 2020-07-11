#include "HertzToSeconds.h"

dsp::HertzToSeconds::HertzToSeconds(Space space) : Processor(Type::HERTZ, Type::SECONDS, space) {}

void dsp::HertzToSeconds::process() {
    Unit::process();
    transform(oneOver);
}
