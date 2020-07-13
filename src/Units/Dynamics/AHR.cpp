#include "AHR.h"

const unsigned int dsp::AHR::RESET_TRIGGER = 0;
const unsigned int dsp::AHR::ATTACK = 1;
const unsigned int dsp::AHR::HOLD = 2;
const unsigned int dsp::AHR::RELEASE = 3;
const unsigned int dsp::AHR::CURRENT_TIME = 1;

dsp::AHR::AHR() : Generator(Type::UNIPOLAR), index(0) {
    pushInput(Type::BINARY);
    pushInput(Type::SECONDS);
    pushInput(Type::SECONDS);
    pushInput(Type::SECONDS);
    pushOutput(Type::SECONDS);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::AHR::getResetTrigger() const {
    return getInput(RESET_TRIGGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::AHR::getAttack() const {
    return getInput(ATTACK);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::AHR::getHold() const {
    return getInput(HOLD);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::AHR::getRelease() const {
    return getInput(RELEASE);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::AHR::getCurrentTime() const {
    return getOutput(CURRENT_TIME);
}

void dsp::AHR::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    index.resize(numChannels, 0);
    value.resize(numChannels, 1.0);
}

void dsp::AHR::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &attackBuffer = getAttack()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &holdBuffer = getHold()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &releaseBuffer = getRelease()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &currentTimeBuffer = getCurrentTime()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (resetTriggerBuffer[k]) {
                index[i] = 0;
                value[i] = 1.0;
            }
            DSP_FLOAT attackPosition = attackBuffer[k] * getSampleRate();
            if (index[i] < attackPosition) {
                value[i] = 1.0;
                outputBuffer[k] = index[i] / attackPosition;
            } else if (index[i] < (attackBuffer[k] + holdBuffer[k]) * getSampleRate()) {
                outputBuffer[k] = value[i] = 1.0;
            } else {
                if (releaseBuffer[k] > 0) {
                    value[i] *= pow(0.001, 1.0 / (releaseBuffer[k] * getSampleRate()));
                } else {
                    value[i] = 0.0;
                }
                outputBuffer[k] = value[i];
            }
            currentTimeBuffer[k] = index[i] * getOneOverSampleRate();
            index[i]++;
        }
    }
}
