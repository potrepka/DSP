#include "SampleAndHold.h"

const unsigned int dsp::SampleAndHold::TRIGGER = 0;

dsp::SampleAndHold::SampleAndHold(Type type) : Processor(type, type) {
    pushInput(Type::BINARY);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SampleAndHold::getTrigger() {
    return getInput(TRIGGER);
}

void dsp::SampleAndHold::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    memory.resize(numChannels, std::numeric_limits<DSP_FLOAT>::quiet_NaN());
}

void dsp::SampleAndHold::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &triggerBuffer = getTrigger()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (triggerBuffer[k]) {
                memory[i] = inputBuffer[k];
            }
            outputBuffer[k] = std::isnan(memory[i]) ? getOutputSignal()->getDefaultValue() : memory[i];
        }
    }
}
