#include "XorGate.h"

dsp::XorGate::XorGate(Space space)
        : Generator(Type::BINARY, space) {}

std::shared_ptr<dsp::InputParameter> dsp::XorGate::pushInput() {
    return Unit::pushInput(Type::BINARY, getOutputSignal()->getSpace(), 0.0);
}

void dsp::XorGate::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        if (getNumInputs() > 0) {
            getOutputSignal()->getChannel(i)->fillBuffer(0.0);
            for (const auto &input : inputs) {
                std::transform(getOutputSignal()->getChannel(i)->getBuffer().begin(),
                               getOutputSignal()->getChannel(i)->getBuffer().end(),
                               input->getChannel(i)->getBuffer().begin(),
                               getOutputSignal()->getChannel(i)->getBuffer().begin(),
                               [](Sample x, Sample y) { return (x || y) && !(x && y); });
            }
        }
    }
}
