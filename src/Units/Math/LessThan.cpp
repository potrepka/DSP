#include "LessThan.h"

dsp::LessThan::LessThan(Type type, Space space)
        : Processor(type, Type::BINARY, space)
        , COMPARISON_SIGNAL(pushInput(type, space)) {}

std::shared_ptr<dsp::InputParameter> dsp::LessThan::getComparisonSignal() const {
    return getInput(COMPARISON_SIGNAL);
}

void dsp::LessThan::process() {
    Unit::process();
    transform(getComparisonSignal(), [](DSP_FLOAT x, DSP_FLOAT y) { return x < y; });
}
