#include "Negative.h"

dsp::Negative::Negative(Connection::Type type, Connection::Space space) : Processor(type, type, space) {
    assert(type != Connection::Type::BINARY);
}

void dsp::Negative::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x) { return -x; });
    }
}
