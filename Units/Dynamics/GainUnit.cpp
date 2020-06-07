#include "GainUnit.h"

const std::size_t dsp::GainUnit::GAIN = 1;

dsp::GainUnit::GainUnit() : Filter(Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::DECIBELS);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::GainUnit::getGain() {
    return getInput(GAIN);
}

void dsp::GainUnit::process() {
    Filter::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getGain()->getChannel(i)->getBuffer().begin(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x, DSP_FLOAT y) { return pow(10.0, x * 0.05) * y; });
    }
}
