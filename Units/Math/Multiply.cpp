#include "Multiply.h"

dsp::Multiply::Multiply(Connection::Type type) : Processor(type, type) {}

void dsp::Multiply::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        getOutputSignal()->getChannel(i)->fillBuffer(1);
        for (const auto &input : inputs) {
            std::transform(getOutputSignal()->getChannel(i)->getBuffer().begin(),
                           getOutputSignal()->getChannel(i)->getBuffer().end(),
                           input->getChannel(i)->getBuffer().begin(),
                           getOutputSignal()->getChannel(i)->getBuffer().begin(),
                           std::multiplies<DSP_FLOAT>());
        }
    }
}