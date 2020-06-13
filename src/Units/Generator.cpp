#include "Generator.h"

const std::size_t dsp::Generator::OUTPUT_SIGNAL = 0;

dsp::Generator::Generator(Connection::Type type) {
    pushOutput(type);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::Generator::getOutputSignal() {
    return getOutput(OUTPUT_SIGNAL);
}

void dsp::Generator::setOutputSignal(std::shared_ptr<OutputParameter> output) {
    setOutput(OUTPUT_SIGNAL, output);
}
