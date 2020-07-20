#include "Phasor.h"

dsp::Phasor::Phasor()
        : Generator(Type::UNIPOLAR)
        , resetTrigger(pushInput(Type::BINARY))
        , frequency(pushInput(Type::HERTZ)) {}

std::shared_ptr<dsp::InputParameter> dsp::Phasor::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::InputParameter> dsp::Phasor::getFrequency() const {
    return frequency;
}

void dsp::Phasor::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    phase.resize(numChannels, 0.0);
}

void dsp::Phasor::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        Array &frequencyBuffer = getFrequency()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Iterator resetTriggerIterator = resetTriggerBuffer.begin();
        Iterator frequencyIterator = frequencyBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
            Vector phaseVector;
            for (int k = 0; k < Vector::Size; ++k) {
                if ((*resetTriggerIterator)[k]) {
                    phase[i] = 0.0;
                }
                phaseVector[k] = phase[i];
                phase[i] += (*frequencyIterator)[k] * getOneOverSampleRate();
            }
            *outputIterator = wrap(phaseVector, 0.0, 1.0);
#else
            if (*resetTriggerIterator) {
                phase[i] = 0.0;
            }
            phase[i] += *frequencyIterator * getOneOverSampleRate();
            *outputIterator = wrap(phase[i], 0.0, 1.0);
#endif
            ++resetTriggerIterator;
            ++frequencyIterator;
            ++outputIterator;
        }
    }
}
