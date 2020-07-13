#include "PinkNoise.h"

dsp::PinkNoise::PinkNoise()
        : Generator(Type::BIPOLAR)
        , seed(1) {}

void dsp::PinkNoise::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    values.resize(numChannels, std::vector<DSP_FLOAT>(7, 0.0));
}

void dsp::PinkNoise::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        DSP_FLOAT &b0 = values[i][0];
        DSP_FLOAT &b1 = values[i][1];
        DSP_FLOAT &b2 = values[i][2];
        DSP_FLOAT &b3 = values[i][3];
        DSP_FLOAT &b4 = values[i][4];
        DSP_FLOAT &b5 = values[i][5];
        DSP_FLOAT &b6 = values[i][6];
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            seed *= 16807;
            DSP_FLOAT white = static_cast<DSP_FLOAT>(seed) * 4.65661287e-10;
            b0 = 0.99886 * b0 + white * 0.0555179;
            b1 = 0.99332 * b1 + white * 0.0750759;
            b2 = 0.96900 * b2 + white * 0.1538520;
            b3 = 0.86650 * b3 + white * 0.3104856;
            b4 = 0.55000 * b4 + white * 0.5329522;
            b5 = -0.7616 * b5 - white * 0.0168980;
            outputBuffer[k] = 0.125 * (b0 + b1 + b2 + b3 + b4 + b5 + b6 + white * 0.5362);
            b6 = white * 0.115926;
        }
    }
}
