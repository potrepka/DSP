#include "AHR.h"

dsp::AHR::AHR()
        : Generator(Type::UNIPOLAR)
        , RESET_TRIGGER(pushInput(Type::BINARY))
        , ATTACK(pushInput(Type::SECONDS))
        , HOLD(pushInput(Type::SECONDS))
        , RELEASE((pushInput(Type::SECONDS)))
        , CURRENT_TIME(pushOutput(Type::SECONDS))
        , index(0) {}

std::shared_ptr<dsp::InputParameter> dsp::AHR::getResetTrigger() const {
    return getInput(RESET_TRIGGER);
}

std::shared_ptr<dsp::InputParameter> dsp::AHR::getAttack() const {
    return getInput(ATTACK);
}

std::shared_ptr<dsp::InputParameter> dsp::AHR::getHold() const {
    return getInput(HOLD);
}

std::shared_ptr<dsp::InputParameter> dsp::AHR::getRelease() const {
    return getInput(RELEASE);
}

std::shared_ptr<dsp::OutputParameter> dsp::AHR::getCurrentTime() const {
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
                value[i] *= pow(0.001, 1.0 / (releaseBuffer[k] * getSampleRate()));
                outputBuffer[k] = value[i];
            }
            currentTimeBuffer[k] = index[i] * getOneOverSampleRate();
            index[i]++;
        }
    }
}
