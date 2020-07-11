#include "OneOver.h"

dsp::OneOver::OneOver(Space space) : Processor(Type::RATIO, Type::RATIO, space) {}

void dsp::OneOver::process() {
    Unit::process();
    transform(oneOver);
}
