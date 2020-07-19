#include "GainScale.h"

dsp::GainScale::GainScale(Type type, Space space)
        : Processor(type, type, space)
        , gain(pushInput(Type::LINEAR, space)) {}

std::shared_ptr<dsp::InputParameter> dsp::GainScale::getGain() const {
    return gain;
}

void dsp::GainScale::process() {
    Unit::process();
    transform(getGain(), [](Sample x, Sample y) { return x * exp2(y); });
}
