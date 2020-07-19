#include "Multiply.h"

dsp::Multiply::Multiply(Type type, Space space)
        : Processor(type, type, space) {
    assert(type != Type::BINARY);
    assert(type != Type::INTEGER);
}

std::shared_ptr<dsp::InputParameter> dsp::Multiply::pushInputBipolar() {
    return Unit::pushInput(Type::BIPOLAR, getInputSignal()->getSpace(), 1.0);
}

std::shared_ptr<dsp::InputParameter> dsp::Multiply::pushInputUnipolar() {
    return Unit::pushInput(Type::UNIPOLAR, getInputSignal()->getSpace(), 1.0);
}

std::shared_ptr<dsp::InputParameter> dsp::Multiply::pushInputRatio() {
    return Unit::pushInput(Type::RATIO, getInputSignal()->getSpace(), 1.0);
}

std::shared_ptr<dsp::InputParameter> dsp::Multiply::pushInputInteger() {
    return Unit::pushInput(Type::INTEGER, getInputSignal()->getSpace(), 1.0);
}

std::shared_ptr<dsp::InputParameter> dsp::Multiply::pushInputBinary() {
    return Unit::pushInput(Type::BINARY, getInputSignal()->getSpace(), 1.0);
}

void dsp::Multiply::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        if (getNumInputs() > 0) {
            getOutputSignal()->getChannel(i)->fillBuffer(1.0);
            for (const auto &input : inputs) {
                std::transform(getOutputSignal()->getChannel(i)->getBuffer().begin(),
                               getOutputSignal()->getChannel(i)->getBuffer().end(),
                               input->getChannel(i)->getBuffer().begin(),
                               getOutputSignal()->getChannel(i)->getBuffer().begin(),
                               std::multiplies<Sample>());
            }
        }
    }
}
