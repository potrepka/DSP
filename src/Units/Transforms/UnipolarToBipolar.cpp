#include "UnipolarToBipolar.h"

dsp::UnipolarToBipolar::UnipolarToBipolar() : Processor(Type::UNIPOLAR, Type::BIPOLAR) {}

void dsp::UnipolarToBipolar::process() {
    Unit::process();
    transform(unipolarToBipolar);
}
