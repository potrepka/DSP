#include "Gate.h"

const std::size_t dsp::Gate::ON_TRIGGER = 0;
const std::size_t dsp::Gate::OFF_TRIGGER = 1;

dsp::Gate::Gate() : Generator(Connection::Type::BINARY) {}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Gate::getOnTrigger() {
    return getInput(ON_TRIGGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Gate::getOffTrigger() {
    return getInput(OFF_TRIGGER);
}

void dsp::Gate::setNumChannelsNoLock(std::size_t numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    state.resize(numChannels, 0);
}

void dsp::Gate::process() {
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
