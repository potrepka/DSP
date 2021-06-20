#include "SamplePlayer.h"

dsp::SamplePlayer::SamplePlayer(Type type)
        : Producer(type)
        , speed(std::make_shared<Input>(Type::RATIO, Space::TIME, 0.0, 1.0))
        , startTime(std::make_shared<Input>(Type::SECONDS))
        , sampleIndex(std::make_shared<Input>(Type::INTEGER))
        , interpolation(std::make_shared<Input>(Type::INTEGER, Space::TIME, Interpolation::MAX, Interpolation::HERMITE))
        , gate(std::make_shared<Input>(Type::BOOLEAN))
        , reset(std::make_shared<Input>(Type::BOOLEAN))
        , currentTime(std::make_shared<Output>(Type::SECONDS)) {
    getInputs().push_back(speed);
    getInputs().push_back(startTime);
    getInputs().push_back(sampleIndex);
    getInputs().push_back(interpolation);
    getInputs().push_back(gate);
    getInputs().push_back(reset);
    getOutputs().push_back(currentTime);
}

std::vector<std::shared_ptr<dsp::Buffer>> &dsp::SamplePlayer::getSamples() {
    return samples;
}

std::shared_ptr<dsp::Input> dsp::SamplePlayer::getSpeed() const {
    return speed;
}

std::shared_ptr<dsp::Input> dsp::SamplePlayer::getStartTime() const {
    return startTime;
}

std::shared_ptr<dsp::Input> dsp::SamplePlayer::getSampleIndex() const {
    return sampleIndex;
}

std::shared_ptr<dsp::Input> dsp::SamplePlayer::getInterpolation() const {
    return interpolation;
}

std::shared_ptr<dsp::Input> dsp::SamplePlayer::getGate() const {
    return gate;
}

std::shared_ptr<dsp::Input> dsp::SamplePlayer::getReset() const {
    return reset;
}

std::shared_ptr<dsp::Output> dsp::SamplePlayer::getCurrentTime() const {
    return currentTime;
}

void dsp::SamplePlayer::setNumOutputChannelsNoLock(size_t numChannels) {
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
        for (size_t channel = 0; channel < getNumChannels(); ++channel) {
            Sample *speedChannel = getSpeed()->getWrapper().getChannelPointer(channel);
            Sample *startTimeChannel = getStartTime()->getWrapper().getChannelPointer(channel);
            Sample *sampleIndexChannel = getSampleIndex()->getWrapper().getChannelPointer(channel);
            Sample *interpolationChannel = getInterpolation()->getWrapper().getChannelPointer(channel);
            Sample *gateChannel = getGate()->getWrapper().getChannelPointer(channel);
            Sample *resetChannel = getReset()->getWrapper().getChannelPointer(channel);
            Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
            Sample *currentTimeChannel = getCurrentTime()->getWrapper().getChannelPointer(channel);
            for (size_t sample = 0; sample < getNumSamples(); ++sample) {
                Sample &interpolation = interpolationChannel[sample];
                if (resetChannel[sample]) {
                    readIndex[channel] = 0.0;
                }
                size_t p = static_cast<size_t>(sampleIndexChannel[sample]);
                if (p < samples.size() && samples[p] != nullptr) {
                    size_t numChannels = samples[p]->getNumChannels();
                    size_t numSamples = samples[p]->getNumSamples();
                    if (numChannels > 0 && numSamples > 0) {
                        Sample *sampleChannel = samples[p]->getWrapper().getChannelPointer(channel % numChannels);
                        if (gateChannel[sample]) {
                            Sample offset = startTimeChannel[sample] * getSampleRate();
                            Sample index = clip(readIndex[channel] + offset, 0.0, numSamples);
                            Array points;
                            switch (static_cast<int>(interpolation)) {
                                case Interpolation::NONE:
                                    outputChannel[sample] = sampleChannel[static_cast<size_t>(index)];
                                    break;
                                case Interpolation::LINEAR: {
                                    size_t k0 = static_cast<size_t>(index);
                                    size_t k1 = (k0 + 1) % numSamples;
                                    points.resize(2);
                                    points[0] = sampleChannel[k0];
                                    points[1] = sampleChannel[k1];
                                    outputChannel[sample] = linear(points.data(), 2, index - k0);
                                } break;
                                case Interpolation::HERMITE: {
                                    size_t k1 = static_cast<size_t>(index);
                                    size_t k2 = (k1 + 1) % numSamples;
                                    size_t k3 = (k1 + 2) % numSamples;
                                    size_t k0 = (k1 + numSamples - 1) % numSamples;
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
