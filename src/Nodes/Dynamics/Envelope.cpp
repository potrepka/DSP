#include "Envelope.h"

dsp::Envelope::Envelope()
        : Producer(Type::RATIO)
        , attackMode(Mode::LINEAR)
        , releaseMode(Mode::LINEAR)
        , resetTrigger(std::make_shared<Input>(Type::INTEGER))
        , gate(std::make_shared<Input>(Type::INTEGER))
        , attack(std::make_shared<Input>(Type::SECONDS))
        , release(std::make_shared<Input>(Type::SECONDS))
        , currentTime(std::make_shared<Output>(Type::SECONDS)) {
    getInputs().push_back(resetTrigger);
    getInputs().push_back(gate);
    getInputs().push_back(attack);
    getInputs().push_back(release);
    getOutputs().push_back(currentTime);
}

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

std::shared_ptr<dsp::Input> dsp::Envelope::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::Input> dsp::Envelope::getGate() const {
    return gate;
}

std::shared_ptr<dsp::Input> dsp::Envelope::getAttack() const {
    return attack;
}

std::shared_ptr<dsp::Input> dsp::Envelope::getRelease() const {
    return release;
}

std::shared_ptr<dsp::Output> dsp::Envelope::getCurrentTime() const {
    return currentTime;
}

void dsp::Envelope::setNumOutputChannelsNoLock(int numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    attackIndex.resize(numChannels, 0);
    releaseIndex.resize(numChannels, 0);
    value.resize(numChannels, 0.0);
}

void dsp::Envelope::processNoLock() {
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *resetTriggerChannel = getResetTrigger()->getWrapper().getChannelPointer(channel);
        Sample *gateChannel = getGate()->getWrapper().getChannelPointer(channel);
        Sample *attackChannel = getAttack()->getWrapper().getChannelPointer(channel);
        Sample *releaseChannel = getRelease()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample *currentTimeChannel = getCurrentTime()->getWrapper().getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            if (gateChannel[sample]) {
                if (resetTriggerChannel[sample]) {
                    attackIndex[channel] = 0;
                    value[channel] = 0.0;
                }
                Sample attackPosition = attackChannel[sample] * getSampleRate();
                switch (attackMode) {
                    case Mode::LINEAR:
                        value[channel] += 1.0 / attackPosition;
                        if (value[channel] > 1.0) {
                            value[channel] = 1.0;
                        }
                        break;
                    case Mode::EXPONENTIAL:
                        value[channel] = 1.0 + pow(0.001, 1.0 / attackPosition) * (value[channel] - 1.0);
                        break;
                }
                currentTimeChannel[sample] = attackIndex[channel] * getOneOverSampleRate();
                ++attackIndex[channel];
                releaseIndex[channel] = 0;
            } else {
                if (resetTriggerChannel[sample]) {
                    releaseIndex[channel] = 0;
                    value[channel] = 1.0;
                }
                Sample releasePosition = releaseChannel[sample] * getSampleRate();
                switch (releaseMode) {
                    case Mode::LINEAR:
                        value[channel] -= 1.0 / releasePosition;
                        if (value[channel] < 0.0) {
                            value[channel] = 0.0;
                        }
                        break;
                    case Mode::EXPONENTIAL: value[channel] *= pow(0.001, 1.0 / releasePosition); break;
                }
                currentTimeChannel[sample] = releaseIndex[channel] * getOneOverSampleRate();
                ++releaseIndex[channel];
                attackIndex[channel] = 0;
            }
            outputChannel[sample] = value[channel];
        }
    }
}
