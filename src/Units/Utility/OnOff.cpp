#include "OnOff.h"

const std::size_t dsp::OnOff::ON_TRIGGER = 0;
const std::size_t dsp::OnOff::OFF_TRIGGER = 1;

dsp::OnOff::OnOff() : Generator(Connection::Type::BINARY) {}

std::shared_ptr<dsp::Unit::InputParameter> dsp::OnOff::getOnTrigger() {
    return getInput(ON_TRIGGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::OnOff::getOffTrigger() {
    return getInput(OFF_TRIGGER);
}

void dsp::OnOff::setNumChannelsNoLock(std::size_t numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    state.resize(numChannels, 0);
}

void dsp::OnOff::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &onTriggerBuffer = getOnTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &offTriggerBuffer = getOffTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (onTriggerBuffer[k]) {
                state[i] = 1.0;
            }
            if (offTriggerBuffer[k]) {
                state[i] = 0.0;
            }
            outputBuffer[k] = state[i];
        }
    }
}
