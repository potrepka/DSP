#include "Envelope.h"

dsp::Envelope::Envelope()
        : Producer(Type::RATIO)
        , attack(std::make_shared<Input>(Type::SECONDS))
        , release(std::make_shared<Input>(Type::SECONDS))
        , attackShape(std::make_shared<Input>(Type::INTEGER, Space::TIME, Shape::MAX))
        , releaseShape(std::make_shared<Input>(Type::INTEGER, Space::TIME, Shape::MAX))
        , gate(std::make_shared<Input>(Type::BOOLEAN))
        , reset(std::make_shared<Input>(Type::BOOLEAN))
        , currentTime(std::make_shared<Output>(Type::SECONDS)) {
    getInputs().push_back(attack);
    getInputs().push_back(release);
    getInputs().push_back(attackShape);
    getInputs().push_back(releaseShape);
    getInputs().push_back(gate);
    getInputs().push_back(reset);
    getOutputs().push_back(currentTime);
}

std::shared_ptr<dsp::Input> dsp::Envelope::getAttack() const {
    return attack;
}

std::shared_ptr<dsp::Input> dsp::Envelope::getRelease() const {
    return release;
}

std::shared_ptr<dsp::Input> dsp::Envelope::getAttackShape() const {
    return attackShape;
}

std::shared_ptr<dsp::Input> dsp::Envelope::getReleaseShape() const {
    return releaseShape;
}

std::shared_ptr<dsp::Input> dsp::Envelope::getGate() const {
    return gate;
}

std::shared_ptr<dsp::Input> dsp::Envelope::getReset() const {
    return reset;
}

std::shared_ptr<dsp::Output> dsp::Envelope::getCurrentTime() const {
    return currentTime;
}

void dsp::Envelope::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    attackIndex.resize(numChannels, 0);
    releaseIndex.resize(numChannels, 0);
    state.resize(numChannels, 0.0);
}

void dsp::Envelope::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *attackChannel = getAttack()->getWrapper().getChannelPointer(channel);
        Sample *releaseChannel = getRelease()->getWrapper().getChannelPointer(channel);
        Sample *attackShapeChannel = getAttackShape()->getWrapper().getChannelPointer(channel);
        Sample *releaseShapeChannel = getReleaseShape()->getWrapper().getChannelPointer(channel);
        Sample *gateChannel = getGate()->getWrapper().getChannelPointer(channel);
        Sample *resetChannel = getReset()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample *currentTimeChannel = getCurrentTime()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample &gate = gateChannel[sample];
            Sample &reset = resetChannel[sample];
            Sample &output = outputChannel[sample];
            Sample &currentTime = currentTimeChannel[sample];
            if (gate) {
                Sample &attack = attackChannel[sample];
                Sample &attackShape = attackShapeChannel[sample];
                if (reset) {
                    attackIndex[channel] = 0;
                    state[channel] = 0.0;
                }
                Sample attackSamples = attack * getSampleRate();
                switch (static_cast<int>(attackShape)) {
                    case Shape::LINEAR:
                        state[channel] += 1.0 / attackSamples;
                        if (state[channel] > 1.0) {
                            state[channel] = 1.0;
                        }
                        break;
                    case Shape::EXPONENTIAL:
                        state[channel] = 1.0 + pow(0.001, 1.0 / attackSamples) * (state[channel] - 1.0);
                        break;
                }
                currentTime = attackIndex[channel] * getOneOverSampleRate();
                ++attackIndex[channel];
                releaseIndex[channel] = 0;
            } else {
                Sample &release = releaseChannel[sample];
                Sample &releaseShape = releaseShapeChannel[sample];
                if (reset) {
                    releaseIndex[channel] = 0;
                    state[channel] = 1.0;
                }
                Sample releaseSamples = release * getSampleRate();
                switch (static_cast<int>(releaseShape)) {
                    case Shape::LINEAR:
                        state[channel] -= 1.0 / releaseSamples;
                        if (state[channel] < 0.0) {
                            state[channel] = 0.0;
                        }
                        break;
                    case Shape::EXPONENTIAL: state[channel] *= pow(0.001, 1.0 / releaseSamples); break;
                }
                currentTime = releaseIndex[channel] * getOneOverSampleRate();
                ++releaseIndex[channel];
                attackIndex[channel] = 0;
            }
            output = state[channel];
        }
    }
}
