#include "Producer.h"

dsp::Producer::Producer(Type type, Domain domain)
    : output(std::make_shared<Output>(type, domain)) {
  getOutputs().push_back(output);
}

std::shared_ptr<dsp::Output> dsp::Producer::getOutput() const { return output; }
