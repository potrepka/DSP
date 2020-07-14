#include "Generator.h"

dsp::Generator::Generator(Type type, Space space)
        : OUTPUT_SIGNAL(pushOutput(type, space)) {}

std::shared_ptr<dsp::OutputParameter> dsp::Generator::getOutputSignal() const {
    return getOutput(OUTPUT_SIGNAL);
}

void dsp::Generator::setOutputSignal(std::shared_ptr<OutputParameter> output) {
    replaceOutput(getOutputSignal(), output);
}
