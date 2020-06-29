#include "Invert.h"

dsp::Invert::Invert(Connection::Type type, Connection::Space space) : Processor(type, type, space) {}

void dsp::Invert::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x) { return -x; });
    }
}
