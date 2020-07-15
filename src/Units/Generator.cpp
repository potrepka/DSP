#include "Generator.h"

dsp::Generator::Generator(Type type, Space space)
        : outputSignal(pushOutput(type, space)) {}

std::shared_ptr<dsp::OutputParameter> dsp::Generator::getOutputSignal() const {
    return outputSignal;
}

void dsp::Generator::setOutputSignal(std::shared_ptr<OutputParameter> output) {
    replaceOutput(outputSignal, outputSignal = output);
}
