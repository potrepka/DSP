#include "Generator.h"

const unsigned int dsp::Generator::OUTPUT_SIGNAL = 0;

dsp::Generator::Generator(Type type, Space space) {
    pushOutput(type, space);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::Generator::getOutputSignal() const {
    return getOutput(OUTPUT_SIGNAL);
}

void dsp::Generator::setOutputSignal(std::shared_ptr<OutputParameter> output) {
    replaceOutput(getOutputSignal(), output);
}
