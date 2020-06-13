#include "WhiteNoise.h"

dsp::WhiteNoise::WhiteNoise() : Generator(Connection::Type::BIPOLAR), seed(1) {}

void dsp::WhiteNoise::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            seed *= 16807;
            outputBuffer[k] = static_cast<DSP_FLOAT>(seed) * 4.65661287e-10;
        }
    }
}
