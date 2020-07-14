#include "Trigger.h"

dsp::Trigger::Trigger()
        : Generator(Type::BINARY)
        , RESET_TRIGGER(pushInput(Type::BINARY))
        , INTERVAL(pushInput(Type::SECONDS))
        , DELAY(pushInput(Type::SECONDS))
        , CURRENT_TIME(pushOutput(Type::SECONDS)) {}

std::shared_ptr<dsp::InputParameter> dsp::Trigger::getResetTrigger() const {
    return getInput(RESET_TRIGGER);
}

std::shared_ptr<dsp::InputParameter> dsp::Trigger::getInterval() const {
    return getInput(INTERVAL);
}

std::shared_ptr<dsp::InputParameter> dsp::Trigger::getDelay() const {
    return getInput(DELAY);
}

std::shared_ptr<dsp::OutputParameter> dsp::Trigger::getCurrentTime() const {
    return getOutput(CURRENT_TIME);
}

void dsp::Trigger::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    index.resize(numChannels, 0.0);
}

void dsp::Trigger::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &intervalBuffer = getInterval()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &delayBuffer = getInterval()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &currentTimeBuffer = getCurrentTime()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (resetTriggerBuffer[k]) {
                index[i] = 0.0;
            }
            outputBuffer[k] = index[i] < 1.0 ? 1.0 : 0.0;
            currentTimeBuffer[k] = index[i] * getOneOverSampleRate();
            DSP_FLOAT interval = abs(intervalBuffer[k] * getSampleRate());
            if (interval == 0.0) {
                index[i] = 0.0;
            } else {
                index[i] += 1.0;
                DSP_FLOAT delayed = index[i] - delayBuffer[k] * getSampleRate();
                if (delayed >= interval) {
                    index[i] -= floor(delayed / interval) * interval;
                }
            }
        }
    }
}
