#include "Consumer.h"

dsp::Consumer::Consumer(Type type, Space space)
        : input(std::make_shared<Input>(type, space)) {
    getInputs().push_back(input);
}

std::shared_ptr<dsp::Input> dsp::Consumer::getInput() const {
    return input;
}
