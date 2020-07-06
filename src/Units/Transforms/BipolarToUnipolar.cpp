#include "BipolarToUnipolar.h"

dsp::BipolarToUnipolar::BipolarToUnipolar() : Processor(Type::BIPOLAR, Type::UNIPOLAR) {}

void dsp::BipolarToUnipolar::process() {
    Unit::process();
    transform(bipolarToUnipolar);
}
