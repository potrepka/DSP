#include "ToBinary.h"

dsp::ToBinary::ToBinary(Connection::Type type) : Processor(type, Connection::Type::BINARY) {}

void dsp::ToBinary::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       toBinary);
    }
}
