#include "ToInteger.h"

dsp::ToInteger::ToInteger(Connection::Type type) : Processor(type, Connection::Type::INTEGER) {}

void dsp::ToInteger::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       toInteger);
    }
}
