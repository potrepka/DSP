#include "Producer.h"

dsp::Producer::Producer(Type type, Space space)
        : output(std::make_shared<Output>(type, space)) {
    getOutputs().push_back(output);
}

std::shared_ptr<dsp::Output> dsp::Producer::getOutput() const {
    return output;
}

void dsp::Producer::setOutput(std::shared_ptr<Output> output) {
    lock();
    std::replace(getOutputs().begin(), getOutputs().end(), this->output, this->output = output);
    unlock();
}
