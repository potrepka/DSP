#include "Quantizer.h"

const std::size_t dsp::Quantizer::QUANTUM = 1;

dsp::Quantizer::Quantizer(Connection::Type type) : Processor(type, type) {
    pushInput(Connection::Type::RATIO);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Quantizer::getQuantum() {
    return getInput(QUANTUM);
}

void dsp::Quantizer::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getQuantum()->getChannel(i)->getBuffer().begin(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x, DSP_FLOAT y) { return y == 0.0 ? x : floor(x / y) * y; });
    }
}
