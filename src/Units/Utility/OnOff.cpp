#include "OnOff.h"

dsp::OnOff::OnOff()
        : Generator(Type::BINARY)
        , onTrigger(pushInput(Type::BINARY))
        , offTrigger(pushInput(Type::BINARY)) {}

std::shared_ptr<dsp::InputParameter> dsp::OnOff::getOnTrigger() const {
    return onTrigger;
}

std::shared_ptr<dsp::InputParameter> dsp::OnOff::getOffTrigger() const {
    return offTrigger;
}

void dsp::OnOff::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    state.resize(numChannels, std::numeric_limits<DSP_FLOAT>::quiet_NaN());
}

void dsp::OnOff::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &onTriggerBuffer = getOnTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &offTriggerBuffer = getOffTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (onTriggerBuffer[k]) {
                state[i] = 1;
            }
            if (offTriggerBuffer[k]) {
                state[i] = 0;
            }
            outputBuffer[k] = std::isnan(state[i]) ? getOutputSignal()->getDefaultValue() : state[i];
        }
    }
}
