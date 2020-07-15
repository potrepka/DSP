#include "LessThan.h"

dsp::LessThan::LessThan(Type type, Space space)
        : Processor(type, Type::BINARY, space)
        , comparisonSignal(pushInput(type, space)) {}

std::shared_ptr<dsp::InputParameter> dsp::LessThan::getComparisonSignal() const {
    return comparisonSignal;
}

void dsp::LessThan::process() {
    Unit::process();
    transform(getComparisonSignal(), [](DSP_FLOAT x, DSP_FLOAT y) { return x < y; });
}
