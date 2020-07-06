#include "Floor.h"

const unsigned int dsp::Floor::DIVISOR = 1;

dsp::Floor::Floor(Connection::Type type, Connection::Space space) : Processor(type, type, space) {
    assert(type != Connection::Type::BINARY);
    pushInput(type == Connection::Type::INTEGER ? Connection::Type::INTEGER : Connection::Type::RATIO, space);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Floor::getDivisor() {
    return getInput(DIVISOR);
}

void dsp::Floor::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::transform(getInputSignal()->getChannel(i)->getBuffer().begin(),
                       getInputSignal()->getChannel(i)->getBuffer().end(),
                       getDivisor()->getChannel(i)->getBuffer().begin(),
                       getOutputSignal()->getChannel(i)->getBuffer().begin(),
                       [](DSP_FLOAT x, DSP_FLOAT y) { return y == 0.0 ? x : floor(x / y) * y; });
    }
}
