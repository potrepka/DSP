#include "AHR.h"

dsp::AHR::AHR()
        : Generator(Type::UNIPOLAR)
        , resetTrigger(pushInput(Type::BINARY))
        , attack(pushInput(Type::SECONDS))
        , hold(pushInput(Type::SECONDS))
        , release((pushInput(Type::SECONDS)))
        , currentTime(pushOutput(Type::SECONDS)) {}

std::shared_ptr<dsp::InputParameter> dsp::AHR::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::InputParameter> dsp::AHR::getAttack() const {
    return attack;
}

std::shared_ptr<dsp::InputParameter> dsp::AHR::getHold() const {
    return hold;
}

std::shared_ptr<dsp::InputParameter> dsp::AHR::getRelease() const {
    return release;
}

std::shared_ptr<dsp::OutputParameter> dsp::AHR::getCurrentTime() const {
    return currentTime;
}

void dsp::AHR::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    index.resize(numChannels, 0);
    value.resize(numChannels, 1.0);
}

void dsp::AHR::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        Array &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        Array &attackBuffer = getAttack()->getChannel(i)->getBuffer();
        Array &holdBuffer = getHold()->getChannel(i)->getBuffer();
        Array &releaseBuffer = getRelease()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Array &currentTimeBuffer = getCurrentTime()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            if (resetTriggerBuffer[k]) {
                index[i] = 0;
                value[i] = 1.0;
            }
            Sample attackPosition = attackBuffer[k] * getSampleRate();
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
