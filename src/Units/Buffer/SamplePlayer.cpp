#include "SamplePlayer.h"

const unsigned int dsp::SamplePlayer::RESET_TRIGGER = 0;
const unsigned int dsp::SamplePlayer::GATE = 1;
const unsigned int dsp::SamplePlayer::START_POSITION = 2;
const unsigned int dsp::SamplePlayer::SPEED = 3;

dsp::SamplePlayer::SamplePlayer() : Generator(Type::BIPOLAR) {
    pushInput(Type::BINARY);
    pushInput(Type::BINARY);
    pushInput(Type::SECONDS);
    pushInput(Type::RATIO);
}

std::shared_ptr<dsp::Buffer> dsp::SamplePlayer::getSample() {
    return sample;
}

void dsp::SamplePlayer::setSample(std::shared_ptr<Buffer> sample) {
    lock();
    this->sample = sample;
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SamplePlayer::getResetTrigger() {
    return getInput(RESET_TRIGGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SamplePlayer::getGate() {
    return getInput(GATE);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SamplePlayer::getStartPosition() {
    return getInput(START_POSITION);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SamplePlayer::getSpeed() {
    return getInput(SPEED);
}

void dsp::SamplePlayer::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    readIndex.resize(numChannels, 0.0);
}

void dsp::SamplePlayer::process() {
    Unit::process();
    if (sample != nullptr) {
        sample->lock();
        if (sample->getNumChannels() > 0) {
            for (unsigned int i = 0; i < getNumChannels(); i++) {
                std::vector<DSP_FLOAT> &channel = sample->getChannel(i % sample->getNumChannels());
                const unsigned int size = static_cast<unsigned int>(channel.size());
                readIndex[i] = wrap(readIndex[i], 0.0, size);
                if (size > 0) {
                    std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &gateBuffer = getGate()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &startPositionBuffer = getStartPosition()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &speedBuffer = getSpeed()->getChannel(i)->getBuffer();
                    for (unsigned int k = 0; k < getBufferSize(); k++) {
                        if (resetTriggerBuffer[k]) {
                            readIndex[i] = wrap(startPositionBuffer[k] * getSampleRate(), 0.0, size);
                        }
                        if (gateBuffer[k]) {
                            const unsigned int k1 = static_cast<int>(readIndex[i]);
                            const unsigned int k0 = (k1 + size - 1) % size;
                            const unsigned int k2 = (k1 + 1) % size;
                            const unsigned int k3 = (k1 + 2) % size;
                            std::vector<DSP_FLOAT> points{channel[k0], channel[k1], channel[k2], channel[k3]};
                            outputBuffer[k] = hermite(points, 1 + readIndex[i] - k1);
                            readIndex[i] = wrap(readIndex[i] + speedBuffer[k], 0.0, size);
                        }
                    }
                }
            }
        }
    }
    sample->unlock();
}
