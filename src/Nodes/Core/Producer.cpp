#include "Producer.h"

dsp::Producer::Producer(Type type, Space space)
        : output(std::make_shared<Output>(type, space)) {
    getOutputs().push_back(output);
}

std::shared_ptr<dsp::Output> dsp::Producer::getOutput() const {
    return output;
}
