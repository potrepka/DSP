#include "Multiply.h"

dsp::Multiply::Multiply(Type type, Space space) : Processor(type, type, space), type(type), space(space) {
    assert(type != Type::BINARY);
    assert(type != Type::INTEGER);
}

void dsp::Multiply::pushInputBipolar() {
    Unit::pushInput(Type::BIPOLAR, space);
}

void dsp::Multiply::pushInputUnipolar() {
    Unit::pushInput(Type::UNIPOLAR, space);
}

void dsp::Multiply::pushInputRatio() {
    Unit::pushInput(Type::RATIO, space);
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
