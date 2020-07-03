#include "Multiply.h"

dsp::Multiply::Multiply(Connection::Type type, Connection::Space space)
        : Processor(type, type, space), type(type), space(space) {}

void dsp::Multiply::pushInputBinary() {
    Unit::pushInput(Connection::Type::BINARY, space);
}

void dsp::Multiply::pushInputInteger() {
    assert(type != Connection::Type::BINARY);
    Unit::pushInput(Connection::Type::INTEGER, space);
}

void dsp::Multiply::pushInputRatio() {
    assert(type != Connection::Type::BINARY);
    assert(type != Connection::Type::INTEGER);
    Unit::pushInput(Connection::Type::RATIO, space);
}

void dsp::Multiply::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        getOutputSignal()->getChannel(i)->fillBuffer(1.0);
        for (const auto &input : inputs) {
            std::transform(getOutputSignal()->getChannel(i)->getBuffer().begin(),
                           getOutputSignal()->getChannel(i)->getBuffer().end(),
                           input->getChannel(i)->getBuffer().begin(),
                           getOutputSignal()->getChannel(i)->getBuffer().begin(),
                           std::multiplies<DSP_FLOAT>());
        }
    }
}
