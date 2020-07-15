#include "Consumer.h"

dsp::Consumer::Consumer(Type type, Space space)
        : inputSignal(pushInput(type, space)) {}

std::shared_ptr<dsp::InputParameter> dsp::Consumer::getInputSignal() const {
    return inputSignal;
}

void dsp::Consumer::setInputSignal(std::shared_ptr<InputParameter> input) {
    replaceInput(inputSignal, inputSignal = input);
}
