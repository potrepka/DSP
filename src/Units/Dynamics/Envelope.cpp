#include "Envelope.h"

dsp::Envelope::Envelope()
        : Generator(Type::UNIPOLAR)
        , resetTrigger(pushInput(Type::BINARY))
        , gate(pushInput(Type::BINARY))
        , attack(pushInput(Type::SECONDS))
        , release(pushInput(Type::SECONDS))
        , currentTime(pushOutput(Type::SECONDS))
        , attackMode(Mode::LINEAR)
        , releaseMode(Mode::LINEAR) {}

dsp::Envelope::Mode dsp::Envelope::getAttackMode() const {
    return attackMode;
}

void dsp::Envelope::setAttackMode(Mode mode) {
    attackMode = mode;
}

dsp::Envelope::Mode dsp::Envelope::getReleaseMode() const {
    return releaseMode;
}

void dsp::Envelope::getReleaseMode(Mode mode) {
    releaseMode = mode;
}

std::shared_ptr<dsp::InputParameter> dsp::Envelope::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::InputParameter> dsp::Envelope::getGate() const {
    return gate;
}

std::shared_ptr<dsp::InputParameter> dsp::Envelope::getAttack() const {
    return attack;
}

std::shared_ptr<dsp::InputParameter> dsp::Envelope::getRelease() const {
    return release;
}

std::shared_ptr<dsp::OutputParameter> dsp::Envelope::getCurrentTime() const {
    return currentTime;
}

void dsp::Envelope::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    attackIndex.resize(numChannels, 0);
    releaseIndex.resize(numChannels, 0);
    value.resize(numChannels, 0.0);
}

void dsp::Envelope::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
        Array &gateBuffer = getGate()->getChannel(i)->getBuffer();
        Array &attackBuffer = getAttack()->getChannel(i)->getBuffer();
        Array &releaseBuffer = getRelease()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        Array &currentTimeBuffer = getCurrentTime()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); ++k) {
            if (gateBuffer[k]) {
                if (resetTriggerBuffer[k]) {
                    attackIndex[i] = 0;
                    value[i] = 0.0;
                }
                Sample attackPosition = attackBuffer[k] * getSampleRate();
                switch (attackMode) {
                    case Mode::LINEAR:
                        value[i] += 1.0 / attackPosition;
                        if (value[i] > 1.0) {
                            value[i] = 1.0;
                        }
                        break;
                    case Mode::EXPONENTIAL: value[i] = 1.0 + pow(0.001, 1.0 / attackPosition) * (value[i] - 1.0); break;
                }
                currentTimeBuffer[k] = attackIndex[i] * getOneOverSampleRate();
                ++attackIndex[i];
                releaseIndex[i] = 0;
            } else {
                if (resetTriggerBuffer[k]) {
                    releaseIndex[i] = 0;
                    value[i] = 1.0;
                }
                Sample releasePosition = releaseBuffer[k] * getSampleRate();
                switch (releaseMode) {
                    case Mode::LINEAR:
                        value[i] -= 1.0 / releasePosition;
                        if (value[i] < 0.0) {
                            value[i] = 0.0;
                        }
                        break;
                    case Mode::EXPONENTIAL: value[i] *= pow(0.001, 1.0 / releasePosition); break;
                }
                currentTimeBuffer[k] = releaseIndex[i] * getOneOverSampleRate();
                ++releaseIndex[i];
                attackIndex[i] = 0;
            }
            outputBuffer[k] = value[i];
        }
    }
}
