#include "Consumer.h"

const unsigned int dsp::Consumer::INPUT_SIGNAL = 0;

dsp::Consumer::Consumer(Connection::Type type, Connection::Space space) {
    pushInput(type, space);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Consumer::getInputSignal() {
    return getInput(INPUT_SIGNAL);
}

void dsp::Consumer::setInputSignal(std::shared_ptr<InputParameter> input) {
    setInput(INPUT_SIGNAL, input);
}
