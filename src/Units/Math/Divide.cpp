#include "Divide.h"

const std::size_t dsp::Divide::DIVISOR = 1;

dsp::Divide::Divide(Connection::Type type) : Processor(type, Connection::Type::RATIO) {
    pushInput(type);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Divide::getDivisor() {
    return getInput(DIVISOR);
}

void dsp::Divide::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getDivisor()->getChannel(i)->getBuffer().begin(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       std::divides<DSP_FLOAT>());
    }
}