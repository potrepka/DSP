#include "WhiteNoise.h"

dsp::WhiteNoise::WhiteNoise()
        : Producer(Type::RATIO) {}

void dsp::WhiteNoise::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    seed.resize(numChannels);
    std::generate(seed.begin(), seed.end(), []() {
        int x = rand();
        return x ? x : 1;
    });
}

void dsp::WhiteNoise::processNoLock() {
    for (size_t channel = 0; channel < getNumOutputChannels(); ++channel) {
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            outputChannel[sample] = static_cast<Sample>(seed[channel] *= 282475249);
        }
    }
    getOutput()->getWrapper().multiplyBy(4.656612873077392578125e-10);
}
