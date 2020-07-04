#include "GainUnit.h"

const std::size_t dsp::GainUnit::GAIN = 1;

dsp::GainUnit::GainUnit() : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::LINEAR);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::GainUnit::getGain() {
    return getInput(GAIN);
}

void dsp::GainUnit::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getGain()->getChannel(i)->getBuffer().begin(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x, DSP_FLOAT y) { return x * exp2(y); });
    }
}
