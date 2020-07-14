#include "GainScale.h"

dsp::GainScale::GainScale(Type type, Space space)
        : Processor(type, type, space)
        , GAIN(pushInput(Type::LINEAR, space)) {}

std::shared_ptr<dsp::InputParameter> dsp::GainScale::getGain() const {
    return getInput(GAIN);
}

void dsp::GainScale::process() {
    Unit::process();
    transform(getGain(), [](DSP_FLOAT x, DSP_FLOAT y) { return x * exp2(y); });
}
