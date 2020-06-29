#include "Identity.h"

dsp::Identity::Identity(Connection::Type inType, Connection::Type outType, Connection::Space space)
        : Processor(inType, outType, space) {}

void dsp::Identity::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        getOutputSignal()->getChannel(i)->getBuffer() = getInputSignal()->getChannel(i)->getBuffer();
    }
}
