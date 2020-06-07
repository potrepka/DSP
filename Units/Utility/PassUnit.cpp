#include "PassUnit.h"

dsp::PassUnit::PassUnit(Connection::Type type) : Filter(type) {}

void dsp::PassUnit::process() {
    Filter::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        getOutputSignal()->getChannel(i)->getBuffer() = getInputSignal()->getChannel(i)->getBuffer();
    }
}
