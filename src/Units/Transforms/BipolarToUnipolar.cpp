#include "BipolarToUnipolar.h"

dsp::BipolarToUnipolar::BipolarToUnipolar() : Processor(Connection::Type::BIPOLAR, Connection::Type::UNIPOLAR) {}

void dsp::BipolarToUnipolar::process() {
    Unit::process();
    transform(bipolarToUnipolar);
}
