#include "Trigger.h"

const std::size_t dsp::Trigger::RESET_TRIGGER = 0;
const std::size_t dsp::Trigger::INTERVAL = 1;
const std::size_t dsp::Trigger::DELAY = 2;

dsp::Trigger::Trigger() : Generator(Connection::Type::UNIPOLAR) {
    pushInput(Connection::Type::BINARY);
    pushInput(Connection::Type::SECONDS);
    pushInput(Connection::Type::SECONDS);
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

void dsp::Trigger::setNumChannelsNoLock(std::size_t numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    phase.resize(numChannels, 0.0);
}

void dsp::Trigger::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &intervalBuffer = getInterval()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &delayBuffer = getInterval()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (resetTriggerBuffer[k]) {
                phase[i] = 1.0;
            }
            if (phase[i] - delayBuffer[k] >= 1.0) {
                phase[i] -= 1.0;
                outputBuffer[k] = 1.0;
            } else {
                outputBuffer[k] = 0.0;
            }
            phase[i] += fmin(1.0, abs(getOneOverSampleRate() / intervalBuffer[k]));
        }
    }
}
