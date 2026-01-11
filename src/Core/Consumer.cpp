#include "Consumer.h"

dsp::Consumer::Consumer(Type type, Domain domain)
    : input(std::make_shared<Input>(type, domain)) {
  getInputs().push_back(input);
}

std::shared_ptr<dsp::Input> dsp::Consumer::getInput() const { return input; }
