#include "SecondsToHertz.h"

dsp::SecondsToHertz::SecondsToHertz() : Processor(Type::SECONDS, Type::HERTZ) {}

void dsp::SecondsToHertz::process() {
    Unit::process();
    transform(oneOver);
}
