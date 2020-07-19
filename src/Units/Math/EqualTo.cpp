#include "EqualTo.h"

dsp::EqualTo::EqualTo(Type type, Space space)
        : Processor(type, Type::BINARY, space)
        , comparisonSignal(pushInput(type, space)) {}

std::shared_ptr<dsp::InputParameter> dsp::EqualTo::getComparisonSignal() const {
    return comparisonSignal;
}

void dsp::EqualTo::process() {
    Unit::process();
    transform(getComparisonSignal(), [](Sample x, Sample y) { return x == y; });
}
