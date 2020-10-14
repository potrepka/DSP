#include "Noise.h"

dsp::Data dsp::Noise::memoryCoefficientsData = dsp::Data(1, 8);
dsp::Data dsp::Noise::noiseCoefficientsData = dsp::Data(1, 8);
dsp::Wrapper dsp::Noise::memoryCoefficients = []() {
    dsp::Sample *pointer = memoryCoefficientsData.getWritePointer(0);
    pointer[0] = 0.99886;
    pointer[1] = 0.99332;
    pointer[2] = 0.96900;
    pointer[3] = 0.86650;
    pointer[4] = 0.55000;
    pointer[5] = -0.7616;
    pointer[6] = 0.0;
    pointer[7] = 1.0;
    return dsp::Wrapper(memoryCoefficientsData);
}();
dsp::Wrapper dsp::Noise::noiseCoefficients = []() {
    dsp::Sample *pointer = noiseCoefficientsData.getWritePointer(0);
    pointer[0] = 0.0555179;
    pointer[1] = 0.0750759;
    pointer[2] = 0.1538520;
    pointer[3] = 0.3104856;
    pointer[4] = 0.5329522;
    pointer[5] = -0.016898;
    pointer[6] = 0.5362;
    pointer[7] = 0.0;
    return dsp::Wrapper(noiseCoefficientsData);
}();
const dsp::Sample dsp::Noise::delayedNoiseCoefficient = 0.115926;

dsp::Noise::Noise()
        : Producer(Type::RATIO)
        , mode(Mode::WHITE)
        , whiteData(0, 0)
        , memoryData(0, 8)
        , white(whiteData)
        , memory(memoryData) {}

dsp::Noise::Mode dsp::Noise::getMode() const {
    return mode;
}

void dsp::Noise::setMode(Mode mode) {
    lock();
    this->mode = mode;
    memory.clear();
    unlock();
}

void dsp::Noise::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    seed.resize(numChannels);
    std::generate(seed.begin(), seed.end(), []() {
        int x = rand();
        return x ? x : 1;
    });
    whiteData.setSize(numChannels, getNumSamples());
    white = Wrapper(whiteData);
    memoryData.setSize(numChannels, 8);
    memoryData.clear();
    memory = Wrapper(memoryData);
}

void dsp::Noise::setNumSamplesNoLock(size_t numSamples) {
    Node::setNumSamplesNoLock(numSamples);
    whiteData.setSize(getNumOutputChannels(), numSamples);
    white = Wrapper(whiteData);
}

void dsp::Noise::processNoLock() {
    for (size_t channel = 0; channel < getNumOutputChannels(); ++channel) {
        Sample *whiteChannel = white.getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            whiteChannel[sample] = static_cast<Sample>(seed[channel] *= 282475249);
        }
    }
    white.multiplyBy(4.656612873077392578125e-10);
    switch (mode) {
        case Mode::WHITE: getOutput()->getWrapper().copyFrom(white); break;
        case Mode::PINK:
            for (size_t channel = 0; channel < getNumOutputChannels(); ++channel) {
                Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
                Sample *whiteChannel = white.getChannelPointer(channel);
                Wrapper memoryWrapper = memory.getSingleChannel(channel);
                for (size_t sample = 0; sample < getNumSamples(); ++sample) {
                    memoryWrapper.multiplyBy(memoryCoefficients);
                    memoryWrapper.addProductOf(noiseCoefficients, whiteChannel[sample]);
                    Sample *memoryChannel = memoryWrapper.getChannelPointer(0);
                    Sample sum = 0.0;
                    for (unsigned int i = 0; i < 8; ++i) {
                        sum += memoryChannel[i];
                    }
                    outputChannel[sample] = 0.125 * sum;
                    memoryWrapper.setSample(0, 7, whiteChannel[sample] * delayedNoiseCoefficient);
                }
            }
            break;
    }
}
