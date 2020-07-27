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

std::shared_ptr<dsp::InputParameter> dsp::Counter::getOffsetCount() const {
    return offset;
}

std::shared_ptr<dsp::InputParameter> dsp::Counter::getSpeed() const {
    return offset;
}

void dsp::Counter::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    memory.resize(numChannels, 0.0);
    index.resize(numChannels, 0.0);
}

void dsp::Counter::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        Array &triggerBuffer = getTrigger()->getChannel(i)->getBuffer();
        Array &offsetCountBuffer = getOffsetCount()->getChannel(i)->getBuffer();
        Array &speedBuffer = getSpeed()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator resetTriggerIterator = resetTriggerBuffer.begin();
        Iterator triggerIterator = triggerBuffer.begin();
        Iterator offsetCountIterator = offsetCountBuffer.begin();
        Iterator speedIterator = speedBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
            Vector memoryVector;
            for (int k = 0; k < Vector::Size; ++k) {
                if ((*resetTriggerIterator)[k]) {
                    index[i] = 0.0;
                }
                if ((*triggerIterator)[k]) {
                    memory[i] = index[i];
                    index[i] += (*speedIterator)[k];
                }
                memoryVector[k] = memory[i];
            }
            *outputIterator = Vc::floor(memoryVector + *offsetCountIterator);
#else
            if (*resetTriggerIterator) {
                index[i] = 0.0;
            }
            if (*triggerIterator) {
                memory[i] = index[i];
                index[i] += *speedIterator;
            }
            *outputIterator = floor(memory[i] + *offsetCountIterator);
#endif
            ++resetTriggerIterator;
            ++triggerIterator;
            ++offsetCountIterator;
            ++speedIterator;
            ++outputIterator;
        }
    }
}
