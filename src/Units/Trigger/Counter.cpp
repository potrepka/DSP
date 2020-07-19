#include "Counter.h"

dsp::Counter::Counter()
        : Generator(Type::INTEGER)
        , resetTrigger(pushInput(Type::BINARY))
        , trigger(pushInput(Type::BINARY))
        , offset(pushInput(Type::INTEGER))
        , speed(pushInput(Type::RATIO)) {}

std::shared_ptr<dsp::InputParameter> dsp::Counter::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::InputParameter> dsp::Counter::getTrigger() const {
    return trigger;
}

std::shared_ptr<dsp::InputParameter> dsp::Counter::getOffset() const {
    return offset;
}

std::shared_ptr<dsp::InputParameter> dsp::Counter::getSpeed() const {
    return offset;
}

void dsp::Counter::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    memory.resize(numChannels, 0);
    index.resize(numChannels, 0);
}

void dsp::Counter::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        Array &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        Array &triggerBuffer = getTrigger()->getChannel(i)->getBuffer();
        Array &offsetBuffer = getOffset()->getChannel(i)->getBuffer();
        Array &speedBuffer = getSpeed()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (resetTriggerBuffer[k]) {
                index[i] = 0;
            }
            if (triggerBuffer[k]) {
                memory[i] = index[i];
                index[i] += speedBuffer[k];
            }
            outputBuffer[k] = floor(memory[i] + offsetBuffer[k]);
        }
    }
}
