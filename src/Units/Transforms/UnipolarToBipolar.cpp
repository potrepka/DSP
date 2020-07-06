#include "UnipolarToBipolar.h"

dsp::UnipolarToBipolar::UnipolarToBipolar() : Processor(Connection::Type::UNIPOLAR, Connection::Type::BIPOLAR) {}

void dsp::UnipolarToBipolar::process() {
    Unit::process();
    transform(unipolarToBipolar);
}
