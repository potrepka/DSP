#include "AHR.h"

const std::size_t dsp::AHR::RESET_TRIGGER = 0;
const std::size_t dsp::AHR::ATTACK = 1;
const std::size_t dsp::AHR::HOLD = 2;
const std::size_t dsp::AHR::RELEASE = 3;

dsp::AHR::AHR() : Generator(Connection::Type::UNIPOLAR), position(0) {
    pushInput(Connection::Type::INTEGER);
    pushInput(Connection::Type::SECONDS);
    pushInput(Connection::Type::SECONDS);
    pushInput(Connection::Type::SECONDS);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::AHR::getResetTrigger() {
    return getInput(RESET_TRIGGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::AHR::getAttack() {
    return getInput(ATTACK);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::AHR::getHold() {
    return getInput(HOLD);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::AHR::getRelease() {
    return getInput(RELEASE);
}

void dsp::AHR::setNumChannelsNoLock(std::size_t numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    position.resize(numChannels, 0);
    value.resize(numChannels, 1.0);
}

void dsp::AHR::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &attackBuffer = getAttack()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &holdBuffer = getHold()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &releaseBuffer = getRelease()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (resetTriggerBuffer[k]) {
                position[i] = 0;
                value[i] = 1.0;
            }
            DSP_FLOAT attackPosition = attackBuffer[k] * getSampleRate();
            if (position[i] < attackPosition) {
                value[i] = 1.0;
                outputBuffer[k] = position[i] / attackPosition;
            } else if (position[i] < (attackBuffer[k] + holdBuffer[k]) * getSampleRate()) {
                outputBuffer[k] = value[i] = 1.0;
            } else {
                value[i] *= pow(0.001, 1.0 / (releaseBuffer[k] * getSampleRate()));
                outputBuffer[k] = value[i];
            }
        }
    }
}
