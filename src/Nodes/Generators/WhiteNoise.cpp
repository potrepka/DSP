#include "WhiteNoise.h"

dsp::WhiteNoise::WhiteNoise()
        : Producer(Type::RATIO) {}

void dsp::WhiteNoise::setNumOutputChannelsNoLock(int numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    seed.resize(numChannels);
    std::generate(seed.begin(), seed.end(), []() {
        int x = rand();
        return x ? x : 1;
    });
}

void dsp::WhiteNoise::processNoLock() {
    for (unsigned int channel = 0; channel < getNumOutputChannels(); ++channel) {
        Sample *outputChannel = getOutput()->getBlock().getChannelPointer(channel);
        for (unsigned int sample = 0; sample < getNumSamples(); ++sample) {
            outputChannel[sample] = static_cast<Sample>(seed[channel] *= 282475249);
        }
    }
    getOutput()->getBlock().multiplyBy(4.656612873077392578125e-10);
}
