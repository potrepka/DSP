#include "Divide.h"

const unsigned int dsp::Divide::DIVISOR = 1;

dsp::Divide::Divide(Connection::Type type, Connection::Space space) : Processor(type, Connection::Type::RATIO, space) {
    pushInput(type, space);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Divide::getDivisor() {
    return getInput(DIVISOR);
}

void dsp::Divide::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getDivisor()->getChannel(i)->getBuffer().begin(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       std::divides<DSP_FLOAT>());
    }
}
