#include "UnipolarToBipolar.h"

dsp::UnipolarToBipolar::UnipolarToBipolar(Space space)
        : Processor(Type::UNIPOLAR, Type::BIPOLAR, space) {}

void dsp::UnipolarToBipolar::process() {
    Unit::process();
    transform(unipolarToBipolar);
}
