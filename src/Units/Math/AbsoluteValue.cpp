#include "AbsoluteValue.h"

dsp::AbsoluteValue::AbsoluteValue(Connection::Type type, Connection::Space space) : Processor(type, type, space) {}

void dsp::AbsoluteValue::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x) { return abs(x); });
    }
}
