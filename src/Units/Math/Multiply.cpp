#include "Multiply.h"

dsp::Multiply::Multiply(Type type, Space space) : Processor(type, type, space) {
    assert(type != Type::BINARY);
    assert(type != Type::INTEGER);
}

void dsp::Multiply::pushInputBipolar() {
    Unit::pushInput(Type::BIPOLAR, getInputSignal()->getSpace(), 1.0);
}

void dsp::Multiply::pushInputUnipolar() {
    Unit::pushInput(Type::UNIPOLAR, getInputSignal()->getSpace(), 1.0);
}

void dsp::Multiply::pushInputRatio() {
    Unit::pushInput(Type::RATIO, getInputSignal()->getSpace(), 1.0);
}

void dsp::Multiply::pushInputInteger() {
    Unit::pushInput(Type::INTEGER, getInputSignal()->getSpace(), 1.0);
}

void dsp::Multiply::pushInputBinary() {
    Unit::pushInput(Type::BINARY, getInputSignal()->getSpace(), 1.0);
}

void dsp::Multiply::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
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
