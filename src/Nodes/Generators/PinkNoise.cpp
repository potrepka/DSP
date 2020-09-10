#include "PinkNoise.h"

dsp::Data dsp::PinkNoise::memoryCoefficientsData = dsp::Data(1, 8);
dsp::Data dsp::PinkNoise::noiseCoefficientsData = dsp::Data(1, 8);
dsp::Wrapper dsp::PinkNoise::memoryCoefficients = []() {
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
dsp::Wrapper dsp::PinkNoise::noiseCoefficients = []() {
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
const dsp::Sample dsp::PinkNoise::delayedNoiseCoefficient = 0.115926;

dsp::PinkNoise::PinkNoise()
        : Producer(Type::RATIO)
        , whiteData(0, 0)
        , memoryData(0, 8)
        , white(whiteData)
        , memory(memoryData) {}

void dsp::PinkNoise::setNumOutputChannelsNoLock(int numChannels) {
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

void dsp::PinkNoise::setNumSamplesNoLock(int numSamples) {
    Node::setNumSamplesNoLock(numSamples);
    whiteData.setSize(getNumOutputChannels(), numSamples);
    white = Wrapper(whiteData);
}

void dsp::PinkNoise::processNoLock() {
    for (int channel = 0; channel < getNumOutputChannels(); ++channel) {
        Sample *whiteChannel = white.getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            whiteChannel[sample] = static_cast<Sample>(seed[channel] *= 282475249);
        }
    }
    white.multiplyBy(4.656612873077392578125e-10);
    for (int channel = 0; channel < getNumOutputChannels(); ++channel) {
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample *whiteChannel = white.getChannelPointer(channel);
        Wrapper memoryWrapper = memory.getSingleChannel(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            memoryWrapper.multiplyBy(memoryCoefficients);
            memoryWrapper.addProductOf(noiseCoefficients, whiteChannel[sample]);
            Sample *memoryChannel = memoryWrapper.getChannelPointer(0);
            Sample sum = 0.0;
            for (int i = 0; i < 8; ++i) {
                sum += memoryChannel[i];
            }
            outputChannel[sample] = 0.125 * sum;
            memoryWrapper.setSample(0, 7, whiteChannel[sample] * delayedNoiseCoefficient);
        }
    }
}
