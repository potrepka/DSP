#include "SwitchIn.h"

dsp::SwitchIn::SwitchIn(Type type, Space space, Sample defaultValue)
        : Processor(type, type, space)
        , inputIndex(Unit::pushInput(Type::INTEGER, space)) {
    getInputSignal()->setDefaultValue(defaultValue);
    getOutputSignal()->setDefaultValue(defaultValue);
}

std::shared_ptr<dsp::InputParameter> dsp::SwitchIn::getInputIndex() const {
    return inputIndex;
}

std::shared_ptr<dsp::InputParameter> dsp::SwitchIn::pushInput() {
    return Unit::pushInput(
            getInputSignal()->getType(), getInputSignal()->getSpace(), getInputSignal()->getDefaultValue());
}

void dsp::SwitchIn::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &inputIndexBuffer = getInputIndex()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); ++k) {
            if (inputIndexBuffer[k] + 1 < getNumInputs()) {
                outputBuffer[k] = getInput(inputIndexBuffer[k] + 1)->getChannel(i)->getBuffer()[k];
            } else {
                outputBuffer[k] = getInputSignal()->getDefaultValue();
            }
        }
    }
}
