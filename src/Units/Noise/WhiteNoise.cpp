#include "WhiteNoise.h"

dsp::WhiteNoise::WhiteNoise()
        : Generator(Type::BIPOLAR) {}

void dsp::WhiteNoise::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    seed.resize(numChannels);
    for (auto &s : seed) {
        s = rand();
    }
}

void dsp::WhiteNoise::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); ++k) {
            seed[i] *= 282475249;
            outputBuffer[k] = static_cast<Sample>(seed[i]) * 4.656612873077392578125e-10;
        }
    }
}
