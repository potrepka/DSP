#include "SamplePlayer.h"

const unsigned int dsp::SamplePlayer::RESET_TRIGGER = 0;
const unsigned int dsp::SamplePlayer::GATE = 1;
const unsigned int dsp::SamplePlayer::START_TIME = 2;
const unsigned int dsp::SamplePlayer::SPEED = 3;
const unsigned int dsp::SamplePlayer::CURRENT_TIME = 1;

dsp::SamplePlayer::SamplePlayer(Type type) : Generator(type), mode(Mode::ONE_SHOT) {
    pushInput(Type::BINARY);
    pushInput(Type::BINARY);
    pushInput(Type::SECONDS);
    pushInput(Type::RATIO);
    pushOutput(Type::SECONDS);
}

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

std::shared_ptr<dsp::Unit::InputParameter> dsp::SamplePlayer::getResetTrigger() const {
    return getInput(RESET_TRIGGER);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SamplePlayer::getGate() const {
    return getInput(GATE);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SamplePlayer::getStartTime() const {
    return getInput(START_TIME);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::SamplePlayer::getSpeed() const {
    return getInput(SPEED);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::SamplePlayer::getCurrentTime() const {
    return getOutput(CURRENT_TIME);
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
                switch (mode) {
                    case Mode::ONE_SHOT: readIndex[i] = clip(readIndex[i], 0.0, size); break;
                    case Mode::LOOP: readIndex[i] = wrap(readIndex[i], 0.0, size); break;
                }
                if (size > 0) {
                    std::vector<DSP_FLOAT> &resetTriggerBuffer = getResetTrigger()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &gateBuffer = getGate()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &startTimeBuffer = getStartTime()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &speedBuffer = getSpeed()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
                    std::vector<DSP_FLOAT> &currentTimeBuffer = getCurrentTime()->getChannel(i)->getBuffer();
                    for (unsigned int k = 0; k < getBufferSize(); k++) {
                        if (resetTriggerBuffer[k]) {
                            readIndex[i] = wrap(startTimeBuffer[k] * getSampleRate(), 0.0, size);
                        }
                        if (gateBuffer[k]) {
                            const unsigned int k1 = static_cast<int>(readIndex[i]);
                            const unsigned int k0 = (k1 + size - 1) % size;
                            const unsigned int k2 = (k1 + 1) % size;
                            const unsigned int k3 = (k1 + 2) % size;
                            std::vector<DSP_FLOAT> points{channel[k0], channel[k1], channel[k2], channel[k3]};
                            outputBuffer[k] = hermite(points, 1 + readIndex[i] - k1);
                            currentTimeBuffer[k] = readIndex[i] * getOneOverSampleRate();
                            readIndex[i] += speedBuffer[k];
                            switch (mode) {
                                case Mode::ONE_SHOT: readIndex[i] = clip(readIndex[i], 0.0, size); break;
                                case Mode::LOOP: readIndex[i] = wrap(readIndex[i], 0.0, size); break;
                            }
                        }
                    }
                }
            }
        }
    }
    sample->unlock();
}
