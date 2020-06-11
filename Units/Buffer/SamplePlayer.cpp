#include "SamplePlayer.h"

const std::size_t dsp::SamplePlayer::RESET_TRIGGER = 0;
const std::size_t dsp::SamplePlayer::GATE = 1;
const std::size_t dsp::SamplePlayer::START_POSITION = 2;
const std::size_t dsp::SamplePlayer::SPEED = 3;

dsp::SamplePlayer::SamplePlayer() : Generator(Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::BINARY);
    pushInput(Connection::Type::BINARY);
    pushInput(Connection::Type::SECONDS);
    pushInput(Connection::Type::RATIO);
}

void dsp::SamplePlayer::setNumChannels(std::size_t numChannels) {
    lock();
    Unit::setNumChannelsNoLock(numChannels);
    samples.resize(numChannels);
    position.resize(numChannels, 0);
    unlock();
}

std::vector<DSP_FLOAT> dsp::SamplePlayer::getSample(std::size_t index) {
    return samples[index];
}

void dsp::SamplePlayer::setSample(std::size_t index, const std::vector<DSP_FLOAT> &sample) {
    lock();
    samples[index] = sample;
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SamplePlayer::getGate() {
    return getInput(GATE);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SamplePlayer::getResetTrigger() {
    return getInput(RESET_TRIGGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SamplePlayer::getStartPosition() {
    return getInput(START_POSITION);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SamplePlayer::getSpeed() {
    return getInput(SPEED);
}

void dsp::SamplePlayer::process() {
    Unit::process();
    for (int i = 0; i < getNumChannels(); i++) {
        if (samples[i].size() > 0) {
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &gateBuffer = getGate()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &startPositionBuffer = getStartPosition()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &speedBuffer = getSpeed()->getChannel(i)->getBuffer();
            for (int k = 0; k < getBufferSize(); k++) {
                if (resetTriggerBuffer[k]) {
                    position[i] = fmod(startPositionBuffer[k] * getSampleRate(), samples[i].size());
                }
                if (gateBuffer[k]) {
                    std::size_t k1 = static_cast<int>(position[i]);
                    std::size_t k0 = (k1 + samples[i].size() - 1) % samples[i].size();
                    std::size_t k2 = (k1 + 1) % samples[i].size();
                    std::size_t k3 = (k1 + 2) % samples[i].size();
                    std::vector<DSP_FLOAT> points{samples[i][k0], samples[i][k1], samples[i][k2], samples[i][k3]};
                    outputBuffer[k] = linear(points, 1 + position[i] - k1);
                    position[i] = fmod(position[i] + speedBuffer[k], samples[i].size());
                }
            }
        }
    }
}
