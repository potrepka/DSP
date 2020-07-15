#include "EqualTo.h"

dsp::EqualTo::EqualTo(Type type, Space space)
        : Processor(type, Type::BINARY, space)
        , comparisonSignal(pushInput(type, space)) {}

std::shared_ptr<dsp::InputParameter> dsp::EqualTo::getComparisonSignal() const {
    return comparisonSignal;
}

void dsp::EqualTo::process() {
    Unit::process();
    transform(getComparisonSignal(), [](DSP_FLOAT x, DSP_FLOAT y) { return x == y; });
}
