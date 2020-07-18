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

unsigned int dsp::SamplePlayer::getNumSamples() const {
    return getNumBuffers();
}

std::shared_ptr<dsp::Buffer> dsp::SamplePlayer::getSample(unsigned int index) const {
    return getBuffer(index);
}

std::vector<std::shared_ptr<dsp::Buffer>> dsp::SamplePlayer::getSamples(unsigned int begin, unsigned int end) const {
    return getBuffers(begin, end);
}

void dsp::SamplePlayer::pushSample(std::shared_ptr<Buffer> sample) {
    pushBuffer(sample);
}

void dsp::SamplePlayer::pushSamples(std::vector<std::shared_ptr<Buffer>> samples) {
    pushBuffers(samples);
}

void dsp::SamplePlayer::replaceSample(std::shared_ptr<Buffer> sample, std::shared_ptr<Buffer> replacement) {
    replaceBuffer(sample, replacement);
}

void dsp::SamplePlayer::removeSample(std::shared_ptr<Buffer> sample) {
    removeBuffer(sample);
}

void dsp::SamplePlayer::removeSamples(std::vector<std::shared_ptr<Buffer>> samples) {
    removeBuffers(samples);
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

std::shared_ptr<dsp::InputParameter> dsp::SamplePlayer::getSampleIndex() const {
    return sampleIndex;
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
    if (collection.size() > 0) {
        for (const auto &buffer : collection) {
            if (buffer != nullptr) {
                buffer->lock();
            }
        }
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &gateBuffer = getGate()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &offsetTimeBuffer = getOffsetTime()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &speedBuffer = getSpeed()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &sampleIndexBuffer = getSampleIndex()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &currentTimeBuffer = getCurrentTime()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                if (resetTriggerBuffer[k]) {
                    readIndex[i] = 0.0;
                }
                DSP_FLOAT p = sampleIndexBuffer[k];
                if (p >= 0 && p < collection.size() && collection[p] != nullptr) {
                    unsigned int numChannels = collection[p]->getNumChannels();
                    unsigned int channelSize = collection[p]->getBufferSize();
                    if (numChannels > 0 && channelSize > 0) {
                        std::vector<DSP_FLOAT> &channel = collection[p]->getChannel(i % numChannels);
                        if (gateBuffer[k]) {
                            DSP_FLOAT offset = offsetTimeBuffer[k] * getSampleRate();
                            DSP_FLOAT index;
                            switch (mode) {
                                case Mode::ONE_SHOT: index = clip(readIndex[i] + offset, 0.0, channelSize); break;
                                case Mode::LOOP: index = wrap(readIndex[i] + offset, 0.0, channelSize); break;
                            }
                            unsigned int k1 = static_cast<unsigned int>(index);
                            unsigned int k0 = (k1 + channelSize - 1) % channelSize;
                            unsigned int k2 = (k1 + 1) % channelSize;
                            unsigned int k3 = (k1 + 2) % channelSize;
                            std::vector<DSP_FLOAT> points{channel[k0], channel[k1], channel[k2], channel[k3]};
                            outputBuffer[k] = hermite(points, 1.0 + readIndex[i] - k1);
                            currentTimeBuffer[k] = readIndex[i] * getOneOverSampleRate();
                            readIndex[i] += speedBuffer[k];
                        }
                    } else if (gateBuffer[k]) {
                        outputBuffer[k] = collection[p]->getDefaultValue();
                    }
                }
            }
        }
        for (const auto &buffer : collection) {
            if (buffer != nullptr) {
                buffer->unlock();
            }
        }
    }
}
