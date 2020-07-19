#include "SwitchOut.h"

dsp::SwitchOut::SwitchOut(Type type, Space space, Sample defaultValue)
        : Processor(type, type, space)
        , outputIndex(Unit::pushInput(Type::INTEGER, space)) {
    getInputSignal()->setDefaultValue(defaultValue);
    getOutputSignal()->setDefaultValue(defaultValue);
}

std::shared_ptr<dsp::InputParameter> dsp::SwitchOut::getOutputIndex() const {
    return outputIndex;
}

std::shared_ptr<dsp::OutputParameter> dsp::SwitchOut::pushOutput() {
    return Unit::pushOutput(
            getOutputSignal()->getType(), getOutputSignal()->getSpace(), getOutputSignal()->getDefaultValue());
}

void dsp::SwitchOut::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        Array &outputIndexBuffer = getOutputIndex()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (outputIndexBuffer[k] < getNumOutputs()) {
                getOutput(outputIndexBuffer[k])->getChannel(i)->getBuffer()[k] = inputBuffer[k];
            }
        }
    }
}
