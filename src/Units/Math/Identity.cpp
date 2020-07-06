#include "Identity.h"

dsp::Identity::Identity(Connection::Type inType, Connection::Type outType, Connection::Space space)
        : Processor(inType, outType, space) {}

void dsp::Identity::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        getOutputSignal()->getChannel(i)->getBuffer() = getInputSignal()->getChannel(i)->getBuffer();
    }
}

dsp::BinaryToLinear::BinaryToLinear(Connection::Space space)
        : Identity(Connection::Type::BINARY, Connection::Type::LINEAR, space) {}

dsp::BinaryToRatio::BinaryToRatio(Connection::Space space)
        : Identity(Connection::Type::BINARY, Connection::Type::RATIO, space) {}

dsp::IntegerToLinear::IntegerToLinear(Connection::Space space)
        : Identity(Connection::Type::INTEGER, Connection::Type::LINEAR, space) {}

dsp::IntegerToRatio::IntegerToRatio(Connection::Space space)
        : Identity(Connection::Type::INTEGER, Connection::Type::RATIO, space) {}
