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
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        Array &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        Array &intervalBuffer = getIntervalDuration()->getChannel(i)->getBuffer();
        Array &delayBuffer = getDelayTime()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Array &currentTimeBuffer = getCurrentTime()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (resetTriggerBuffer[k] || intervalBuffer[k] == 0.0) {
                index[i] = 0.0;
            } else {
                Sample interval = abs(intervalBuffer[k] * getSampleRate());
                Sample delayed = index[i] - delayBuffer[k] * getSampleRate();
                if (delayed >= interval) {
                    index[i] -= floor(delayed / interval) * interval;
                }
            }
            outputBuffer[k] = index[i] < 1.0 ? 1.0 : 0.0;
            currentTimeBuffer[k] = index[i] * getOneOverSampleRate();
            index[i] += 1.0;
        }
    }
}
