#include "Identity.h"

dsp::Identity::Identity(Connection::Type inType, Connection::Type outType) : Processor(inType, outType) {}

void dsp::Identity::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        getOutputSignal()->getChannel(i)->getBuffer() = getInputSignal()->getChannel(i)->getBuffer();
    }
}
