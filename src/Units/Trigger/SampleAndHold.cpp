#include "SampleAndHold.h"

dsp::SampleAndHold::SampleAndHold(Type type)
        : Processor(type, type)
        , trigger(pushInput(Type::BINARY)) {}

std::shared_ptr<dsp::InputParameter> dsp::SampleAndHold::getTrigger() const {
    return trigger;
}

void dsp::SampleAndHold::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    memory.resize(numChannels, std::numeric_limits<Sample>::quiet_NaN());
}

void dsp::SampleAndHold::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        Array &triggerBuffer = getTrigger()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator inputIterator = inputBuffer.begin();
        Iterator triggerIterator = triggerBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
            Vector memoryVector;
            for (int k = 0; k < Vector::Size; ++k) {
                if ((*triggerIterator)[k]) {
                    memory[i] = (*inputIterator)[k];
                }
                memoryVector[k] = memory[i];
            }
            *outputIterator =
                    Vc::iif(Vc::isnan(memoryVector), Vector(getOutputSignal()->getDefaultValue()), memoryVector);
#else
            if (*triggerIterator) {
                memory[i] = *inputIterator;
            }
            *outputIterator = std::isnan(memory[i]) ? getOutputSignal()->getDefaultValue() : memory[i];
#endif
            ++inputIterator;
            ++triggerIterator;
            ++outputIterator;
        }
    }
}
