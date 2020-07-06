#include "SamplePlayer.h"

const unsigned int dsp::SamplePlayer::RESET_TRIGGER = 0;
const unsigned int dsp::SamplePlayer::GATE = 1;
const unsigned int dsp::SamplePlayer::START_POSITION = 2;
const unsigned int dsp::SamplePlayer::SPEED = 3;

dsp::SamplePlayer::SamplePlayer() : Generator(Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::BINARY);
    pushInput(Connection::Type::BINARY);
    pushInput(Connection::Type::SECONDS);
    pushInput(Connection::Type::RATIO);
}

std::vector<DSP_FLOAT> dsp::SamplePlayer::getSample(unsigned int channel) {
    return samples[channel];
}

void dsp::SamplePlayer::setSample(unsigned int channel, const std::vector<DSP_FLOAT> &sample) {
    lock();
    samples[channel] = sample;
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

void dsp::SamplePlayer::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    samples.resize(numChannels);
    position.resize(numChannels, 0);
}

void dsp::SamplePlayer::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        const unsigned int size = static_cast<unsigned int>(samples[i].size());
        if (size > 0) {
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &gateBuffer = getGate()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &startPositionBuffer = getStartPosition()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &speedBuffer = getSpeed()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                if (resetTriggerBuffer[k]) {
                    position[i] = wrap(startPositionBuffer[k] * getSampleRate(), 0.0, size);
                }
                if (gateBuffer[k]) {
                    const unsigned int k1 = static_cast<int>(position[i]);
                    const unsigned int k0 = (k1 + size - 1) % size;
                    const unsigned int k2 = (k1 + 1) % size;
                    const unsigned int k3 = (k1 + 2) % size;
                    std::vector<DSP_FLOAT> points{samples[i][k0], samples[i][k1], samples[i][k2], samples[i][k3]};
                    outputBuffer[k] = linear(points, 1 + position[i] - k1);
                    position[i] = wrap(position[i] + speedBuffer[k], 0.0, size);
                }
            }
        }
    }
}
