#include "Consumer.h"

const std::size_t dsp::Consumer::INPUT_SIGNAL = 0;

dsp::Consumer::Consumer(Connection::Type type) {
    pushInput(type);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Consumer::getInputSignal() {
    return getInput(INPUT_SIGNAL);
}

void dsp::Consumer::setInputSignal(std::shared_ptr<InputParameter> input) {
    setInput(INPUT_SIGNAL, input);
}

void dsp::Consumer::process() {
    Unit::process();
}
