#include "Consumer.h"

dsp::Consumer::Consumer(Type type, Space space)
        : INPUT_SIGNAL(pushInput(type, space)) {}

std::shared_ptr<dsp::InputParameter> dsp::Consumer::getInputSignal() const {
    return getInput(INPUT_SIGNAL);
}

void dsp::Consumer::setInputSignal(std::shared_ptr<InputParameter> input) {
    replaceInput(getInputSignal(), input);
}
