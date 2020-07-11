#include "SecondsToHertz.h"

dsp::SecondsToHertz::SecondsToHertz(Space space) : Processor(Type::SECONDS, Type::HERTZ, space) {}

void dsp::SecondsToHertz::process() {
    Unit::process();
    transform(oneOver);
}
