#include "SamplePlayer.h"

dsp::SamplePlayer::SamplePlayer(Type type)
        : Producer(type)
        , interpolation(Interpolation::HERMITE)
        , resetTrigger(std::make_shared<Input>(Type::INTEGER))
        , gate(std::make_shared<Input>(Type::INTEGER))
        , sampleIndex(std::make_shared<Input>(Type::INTEGER))
        , speed(std::make_shared<Input>(Type::RATIO, Space::TIME, 1.0))
        , offsetTime(std::make_shared<Input>(Type::SECONDS))
        , currentTime(std::make_shared<Output>(Type::SECONDS)) {
    getInputs().push_back(resetTrigger);
    getInputs().push_back(gate);
    getInputs().push_back(sampleIndex);
    getInputs().push_back(speed);
    getInputs().push_back(offsetTime);
    getOutputs().push_back(currentTime);
}

dsp::Interpolation dsp::SamplePlayer::getInterpolation() const {
    return interpolation;
}

void dsp::SamplePlayer::setInterpolation(Interpolation interpolation) {
    lock();
    this->interpolation = interpolation;
    unlock();
}

std::vector<std::shared_ptr<dsp::Buffer>> &dsp::SamplePlayer::getSamples() {
    return samples;
}

std::shared_ptr<dsp::Input> dsp::SamplePlayer::getResetTrigger() const {
    return resetTrigger;
}

std::shared_ptr<dsp::Input> dsp::SamplePlayer::getGate() const {
    return gate;
}

std::shared_ptr<dsp::Input> dsp::SamplePlayer::getSampleIndex() const {
    return sampleIndex;
}

std::shared_ptr<dsp::Input> dsp::SamplePlayer::getSpeed() const {
    return speed;
}

std::shared_ptr<dsp::Input> dsp::SamplePlayer::getOffsetTime() const {
    return offsetTime;
}

std::shared_ptr<dsp::Output> dsp::SamplePlayer::getCurrentTime() const {
    return currentTime;
}

void dsp::SamplePlayer::setNumOutputChannelsNoLock(int numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    readIndex.resize(numChannels, 0.0);
}

void dsp::SamplePlayer::processNoLock() {
    if (samples.size() > 0) {
        for (const auto &sample : samples) {
            if (sample != nullptr) {
                sample->lock();
            }
        }
        for (int channel = 0; channel < getNumChannels(); ++channel) {
            Sample *resetTriggerChannel = getResetTrigger()->getBlock().getChannelPointer(channel);
            Sample *gateChannel = getGate()->getBlock().getChannelPointer(channel);
            Sample *sampleIndexChannel = getSampleIndex()->getBlock().getChannelPointer(channel);
            Sample *speedChannel = getSpeed()->getBlock().getChannelPointer(channel);
            Sample *offsetTimeChannel = getOffsetTime()->getBlock().getChannelPointer(channel);
            Sample *outputChannel = getOutput()->getBlock().getChannelPointer(channel);
            Sample *currentTimeChannel = getCurrentTime()->getBlock().getChannelPointer(channel);
            for (int sample = 0; sample < getNumSamples(); ++sample) {
                if (resetTriggerChannel[sample]) {
                    readIndex[channel] = 0.0;
                }
                int p = static_cast<int>(sampleIndexChannel[sample]);
                if (p >= 0 && p < samples.size() && samples[p] != nullptr) {
                    int numChannels = samples[p]->getNumChannels();
                    int numSamples = samples[p]->getNumSamples();
                    if (numChannels > 0 && numSamples > 0) {
                        Sample *sampleChannel = samples[p]->getBlock().getChannelPointer(channel % numChannels);
                        if (gateChannel[sample]) {
                            Sample offset = offsetTimeChannel[sample] * getSampleRate();
                            Sample index = clip(readIndex[channel] + offset, 0.0, numSamples);
                            Array points;
                            switch (interpolation) {
                                case Interpolation::NONE:
                                    outputChannel[sample] = sampleChannel[static_cast<int>(index)];
                                    break;
                                case Interpolation::LINEAR: {
                                    int k0 = static_cast<int>(index);
                                    int k1 = (k0 + 1) % numSamples;
                                    points.resize(2);
                                    points[0] = sampleChannel[k0];
                                    points[1] = sampleChannel[k1];
                                    outputChannel[sample] = linear(points.data(), 2, index - k0);
                                } break;
                                case Interpolation::HERMITE: {
                                    int k1 = static_cast<int>(index);
                                    int k2 = (k1 + 1) % numSamples;
                                    int k3 = (k1 + 2) % numSamples;
                                    int k0 = (k1 + numSamples - 1) % numSamples;
                                    points.resize(4);
                                    points[0] = sampleChannel[k0];
                                    points[1] = sampleChannel[k1];
                                    points[2] = sampleChannel[k2];
                                    points[3] = sampleChannel[k3];
                                    outputChannel[sample] = hermite(points.data(), 4, 1.0 + index - k1);
                                } break;
                            }
                            currentTimeChannel[sample] = index * getOneOverSampleRate();
                            readIndex[channel] += speedChannel[sample];
                        }
                    } else if (gateChannel[sample]) {
                        outputChannel[sample] = samples[p]->getDefaultValue();
                    }
                }
            }
        }
        for (const auto &sample : samples) {
            if (sample != nullptr) {
                sample->unlock();
            }
        }
    }
}
