#include "BipolarToUnipolar.h"

dsp::BipolarToUnipolar::BipolarToUnipolar(Space space)
        : Processor(Type::BIPOLAR, Type::UNIPOLAR, space) {}

void dsp::BipolarToUnipolar::process() {
    Unit::process();
    transform(bipolarToUnipolar);
}
