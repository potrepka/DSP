#include "AbsoluteValue.h"

dsp::AbsoluteValue::AbsoluteValue() : Processor(Connection::Type::BIPOLAR, Connection::Type::UNIPOLAR) {}

void dsp::AbsoluteValue::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x) { return x < 0 ? -x : x; });
    }
}
