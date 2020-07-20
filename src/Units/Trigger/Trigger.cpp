#include "Trigger.h"

dsp::Trigger::Trigger()
        : Generator(Type::BINARY)
        , resetTrigger(pushInput(Type::BINARY))
        , intervalDuration(pushInput(Type::SECONDS))
        , delayTime(pushInput(Type::SECONDS))
        , currentTime(pushOutput(Type::SECONDS)) {}

std::shared_ptr<dsp::InputParameter> dsp::Trigger::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::InputParameter> dsp::Trigger::getIntervalDuration() const {
    return intervalDuration;
}

std::shared_ptr<dsp::InputParameter> dsp::Trigger::getDelayTime() const {
    return delayTime;
}

std::shared_ptr<dsp::OutputParameter> dsp::Trigger::getCurrentTime() const {
    return currentTime;
}

void dsp::Trigger::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    index.resize(numChannels, 0.0);
}

void dsp::Trigger::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        Array &intervalDurationBuffer = getIntervalDuration()->getChannel(i)->getBuffer();
        Array &delayTimeBuffer = getDelayTime()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Array &currentTimeBuffer = getCurrentTime()->getChannel(i)->getBuffer();
        Iterator resetTriggerIterator = resetTriggerBuffer.begin();
        Iterator intervalDurationIterator = intervalDurationBuffer.begin();
        Iterator delayTimeIterator = delayTimeBuffer.begin();
        Iterator outputIterator = outputBuffer.begin();
        Iterator currentTimeIterator = currentTimeBuffer.begin();
        while (outputIterator != outputBuffer.end()) {
#if DSP_USE_VC
            Vector indexVector;
            for (int k = 0; k < Vector::Size; ++k) {
                if ((*resetTriggerIterator)[k] || (*intervalDurationIterator)[k] == 0.0) {
                    index[i] = 0.0;
                }
                indexVector[k] = index[i];
                index[i] += 1.0;
            }
            Vector interval = Vc::abs(*intervalDurationIterator * getSampleRate());
            Vector delayed = indexVector - *delayTimeIterator * getSampleRate();
            indexVector(delayed >= interval) -= Vc::floor(delayed / interval) * interval;
            *outputIterator = Vc::iif(indexVector < Vector::One(), Vector::One(), Vector::Zero());
            *currentTimeIterator = indexVector * getOneOverSampleRate();
            index[i] = indexVector[Vector::Size - 1] + 1.0;
#else
            if (*resetTriggerIterator || *intervalDurationIterator == 0.0) {
                index[i] = 0.0;
            } else {
                Sample interval = abs(*intervalDurationIterator * getSampleRate());
                Sample delayed = index[i] - *delayTimeIterator * getSampleRate();
                if (delayed >= interval) {
                    index[i] -= floor(delayed / interval) * interval;
                }
            }
            *outputIterator = index[i] < 1.0 ? 1.0 : 0.0;
            *currentTimeIterator = index[i] * getOneOverSampleRate();
            index[i] += 1.0;
#endif
            ++resetTriggerIterator;
            ++intervalDurationIterator;
            ++delayTimeIterator;
            ++outputIterator;
            ++currentTimeIterator;
        }
    }
}
