#include "SamplePlayer.h"

dsp::SamplePlayer::SamplePlayer(Type type)
        : Generator(type)
        , resetTrigger(pushInput(Type::BINARY))
        , gate(pushInput(Type::BINARY))
        , offsetTime(pushInput(Type::SECONDS))
        , speed(pushInput(Type::RATIO, Space::TIME, 1.0))
        , currentTime(pushOutput(Type::SECONDS))
        , mode(Mode::ONE_SHOT) {}

dsp::SamplePlayer::Mode dsp::SamplePlayer::getMode() const {
    return mode;
}

void dsp::SamplePlayer::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::Buffer> dsp::SamplePlayer::getSample() const {
    return sample;
}

void dsp::SamplePlayer::setSample(std::shared_ptr<Buffer> sample) {
    lock();
    this->sample = sample;
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::SamplePlayer::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::InputParameter> dsp::SamplePlayer::getGate() const {
    return gate;
}

std::shared_ptr<dsp::InputParameter> dsp::SamplePlayer::getOffsetTime() const {
    return offsetTime;
}

std::shared_ptr<dsp::InputParameter> dsp::SamplePlayer::getSpeed() const {
    return speed;
}

std::shared_ptr<dsp::OutputParameter> dsp::SamplePlayer::getCurrentTime() const {
    return currentTime;
}

void dsp::SamplePlayer::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    readIndex.resize(numChannels, 0.0);
}

void dsp::SamplePlayer::process() {
    Unit::process();
    if (sample != nullptr) {
        sample->lock();
        unsigned int numChannels = sample->getNumChannels();
        unsigned int channelSize = sample->getBufferSize();
        if (numChannels > 0) {
            for (unsigned int i = 0; i < getNumChannels(); i++) {
                switch (mode) {
                    case Mode::ONE_SHOT: readIndex[i] = clip(readIndex[i], 0.0, channelSize); break;
                    case Mode::LOOP: readIndex[i] = wrap(readIndex[i], 0.0, channelSize); break;
                }
                if (channelSize > 0) {
                    std::vector<DSP_FLOAT> &channel = sample->getChannel(i % numChannels);
                    std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &gateBuffer = getGate()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &offsetTimeBuffer = getOffsetTime()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &speedBuffer = getSpeed()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &currentTimeBuffer = getCurrentTime()->getChannel(i)->getBuffer();
                    for (unsigned int k = 0; k < getBufferSize(); k++) {
                        if (resetTriggerBuffer[k]) {
                            readIndex[i] = 0.0;
                        }
                        if (gateBuffer[k]) {
                            DSP_FLOAT index =
                                    wrap(readIndex[i] + offsetTimeBuffer[k] * getSampleRate(), 0.0, channelSize);
                            unsigned int k1 = static_cast<unsigned int>(index);
                            unsigned int k0 = (k1 + channelSize - 1) % channelSize;
                            unsigned int k2 = (k1 + 1) % channelSize;
                            unsigned int k3 = (k1 + 2) % channelSize;
                            std::vector<DSP_FLOAT> points{channel[k0], channel[k1], channel[k2], channel[k3]};
                            outputBuffer[k] = hermite(points, 1.0 + readIndex[i] - k1);
                            currentTimeBuffer[k] = readIndex[i] * getOneOverSampleRate();
                            readIndex[i] += speedBuffer[k];
                            switch (mode) {
                                case Mode::ONE_SHOT: readIndex[i] = clip(readIndex[i], 0.0, channelSize); break;
                                case Mode::LOOP: readIndex[i] = wrap(readIndex[i], 0.0, channelSize); break;
                            }
                        }
                    }
                }
            }
        }
    }
    sample->unlock();
}
