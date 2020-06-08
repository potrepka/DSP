#include "PassUnit.h"

dsp::PassUnit::PassUnit(Connection::Type type) : Processor(type, type) {}

void dsp::PassUnit::process() {
    Processor::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        getOutputSignal()->getChannel(i)->getBuffer() = getInputSignal()->getChannel(i)->getBuffer();
    }
}
