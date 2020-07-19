#include "LessThan.h"

dsp::LessThan::LessThan(Type type, Space space)
        : Processor(type, Type::BINARY, space)
        , comparisonSignal(pushInput(type, space)) {}

std::shared_ptr<dsp::InputParameter> dsp::LessThan::getComparisonSignal() const {
    return comparisonSignal;
}

void dsp::LessThan::process() {
    Unit::process();
    transform(getComparisonSignal(), [](Sample x, Sample y) { return x < y; });
}
