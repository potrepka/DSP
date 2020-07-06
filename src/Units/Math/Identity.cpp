#include "Identity.h"

dsp::Identity::Identity(Type inType, Type outType, Space space) : Processor(inType, outType, space) {}

void dsp::Identity::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        getOutputSignal()->getChannel(i)->getBuffer() = getInputSignal()->getChannel(i)->getBuffer();
    }
}

dsp::BinaryToLinear::BinaryToLinear(Space space) : Identity(Type::BINARY, Type::LINEAR, space) {}

dsp::BinaryToRatio::BinaryToRatio(Space space) : Identity(Type::BINARY, Type::RATIO, space) {}

dsp::IntegerToLinear::IntegerToLinear(Space space) : Identity(Type::INTEGER, Type::LINEAR, space) {}

dsp::IntegerToRatio::IntegerToRatio(Space space) : Identity(Type::INTEGER, Type::RATIO, space) {}
