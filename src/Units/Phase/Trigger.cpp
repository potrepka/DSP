#include "Trigger.h"

const unsigned int dsp::Trigger::RESET_TRIGGER = 0;
const unsigned int dsp::Trigger::INTERVAL = 1;
const unsigned int dsp::Trigger::DELAY = 2;

dsp::Trigger::Trigger() : Generator(Type::BINARY) {
    pushInput(Type::BINARY);
    pushInput(Type::SECONDS);
    pushInput(Type::SECONDS);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Trigger::getResetTrigger() {
    return getInput(RESET_TRIGGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Trigger::getInterval() {
    return getInput(INTERVAL);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Trigger::getDelay() {
    return getInput(DELAY);
}

void dsp::Trigger::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    sampleCount.resize(numChannels, 0.0);
}

void dsp::Trigger::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &intervalBuffer = getInterval()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &delayBuffer = getInterval()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (resetTriggerBuffer[k]) {
                sampleCount[i] = 0.0;
            }
            outputBuffer[k] = sampleCount[i] < 1.0 ? 1.0 : 0.0;
            DSP_FLOAT interval = abs(intervalBuffer[k] * getSampleRate());
            if (interval == 0.0) {
                sampleCount[i] = 0.0;
            } else {
                sampleCount[i] += 1.0;
                DSP_FLOAT delayed = sampleCount[i] - delayBuffer[k] * getSampleRate();
                if (delayed >= interval) {
                    sampleCount[i] -= floor(delayed / interval) * interval;
                }
            }
        }
    }
}
