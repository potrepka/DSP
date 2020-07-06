#include "GainUnit.h"

const unsigned int dsp::GainUnit::GAIN = 1;

dsp::GainUnit::GainUnit(Connection::Space space)
        : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR, space) {
    pushInput(Connection::Type::LINEAR, space);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::GainUnit::getGain() {
    return getInput(GAIN);
}

void dsp::GainUnit::process() {
    Unit::process();
    transform(getGain(), [](DSP_FLOAT x, DSP_FLOAT y) { return x * exp2(y); });
}
