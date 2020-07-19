#include "WhiteNoise.h"

dsp::WhiteNoise::WhiteNoise()
        : Generator(Type::BIPOLAR)
        , seed(1) {}

void dsp::WhiteNoise::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<Sample> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            seed *= 16807;
            outputBuffer[k] = static_cast<Sample>(seed) * 4.65661287e-10;
        }
    }
}
