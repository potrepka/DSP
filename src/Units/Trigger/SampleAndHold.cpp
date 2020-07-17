#include "SampleAndHold.h"

dsp::SampleAndHold::SampleAndHold(Type type)
        : Processor(type, type)
        , trigger(pushInput(Type::BINARY)) {}

std::shared_ptr<dsp::InputParameter> dsp::SampleAndHold::getTrigger() const {
    return trigger;
}

void dsp::SampleAndHold::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    memory.resize(numChannels, std::numeric_limits<DSP_FLOAT>::quiet_NaN());
}

void dsp::SampleAndHold::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &triggerBuffer = getTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (triggerBuffer[k]) {
                memory[i] = inputBuffer[k];
            }
            outputBuffer[k] = std::isnan(memory[i]) ? getOutputSignal()->getDefaultValue() : memory[i];
        }
    }
}