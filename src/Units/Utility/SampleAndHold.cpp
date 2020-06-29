#include "SampleAndHold.h"

const std::size_t dsp::SampleAndHold::TRIGGER = 0;

dsp::SampleAndHold::SampleAndHold(Connection::Type type) : Processor(type, type) {
    pushInput(Connection::Type::BINARY);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SampleAndHold::getTrigger() {
    return getInput(TRIGGER);
}

void dsp::SampleAndHold::setNumChannelsNoLock(std::size_t numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    memory.resize(numChannels, 0);
}

void dsp::SampleAndHold::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &triggerBuffer = getTrigger()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (triggerBuffer[k]) {
                memory[i] = inputBuffer[k];
            }
            outputBuffer[k] = memory[i];
        }
    }
}
