#include "PinkNoise.h"

const dsp::Sample dsp::PinkNoise::VALUE_COEFFICIENT_0 = 0.99886;
const dsp::Sample dsp::PinkNoise::VALUE_COEFFICIENT_1 = 0.99332;
const dsp::Sample dsp::PinkNoise::VALUE_COEFFICIENT_2 = 0.96900;
const dsp::Sample dsp::PinkNoise::VALUE_COEFFICIENT_3 = 0.86650;
const dsp::Sample dsp::PinkNoise::VALUE_COEFFICIENT_4 = 0.55000;
const dsp::Sample dsp::PinkNoise::VALUE_COEFFICIENT_5 = -0.7616;

const dsp::Sample dsp::PinkNoise::NOISE_COEFFICIENT_0 = 0.0555179;
const dsp::Sample dsp::PinkNoise::NOISE_COEFFICIENT_1 = 0.0750759;
const dsp::Sample dsp::PinkNoise::NOISE_COEFFICIENT_2 = 0.1538520;
const dsp::Sample dsp::PinkNoise::NOISE_COEFFICIENT_3 = 0.3104856;
const dsp::Sample dsp::PinkNoise::NOISE_COEFFICIENT_4 = 0.5329522;
const dsp::Sample dsp::PinkNoise::NOISE_COEFFICIENT_5 = -0.016898;
const dsp::Sample dsp::PinkNoise::NOISE_COEFFICIENT_6 = 0.5362;
const dsp::Sample dsp::PinkNoise::NOISE_COEFFICIENT_7 = 0.115926;

dsp::PinkNoise::PinkNoise()
        : Generator(Type::BIPOLAR) {
#if DSP_USE_VC
    valueCoefficients[0] = VALUE_COEFFICIENT_0;
    valueCoefficients[1] = VALUE_COEFFICIENT_1;
    valueCoefficients[2] = VALUE_COEFFICIENT_2;
    valueCoefficients[3] = VALUE_COEFFICIENT_3;
    valueCoefficients[4] = VALUE_COEFFICIENT_4;
    valueCoefficients[5] = VALUE_COEFFICIENT_5;
    valueCoefficients[6] = 0.0;
    valueCoefficients[7] = 1.0;

    noiseCoefficients[0] = NOISE_COEFFICIENT_0;
    noiseCoefficients[1] = NOISE_COEFFICIENT_1;
    noiseCoefficients[2] = NOISE_COEFFICIENT_2;
    noiseCoefficients[3] = NOISE_COEFFICIENT_3;
    noiseCoefficients[4] = NOISE_COEFFICIENT_4;
    noiseCoefficients[5] = NOISE_COEFFICIENT_5;
    noiseCoefficients[6] = NOISE_COEFFICIENT_6;
    noiseCoefficients[7] = 0.0;
#endif
}

void dsp::PinkNoise::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    seed.resize(numChannels);
    for (auto &s : seed) {
        s = rand();
    }
#if DSP_USE_VC
    values.resize(numChannels, Vc::Memory<Vector, 8>() = 0.0);
#else
    values.resize(numChannels, Array(8, 0.0));
#endif
}

void dsp::PinkNoise::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
#if DSP_USE_VC
        Vc::Memory<Vector, 8> &b = values[i];
#else
        Array &b = values[i];
#endif
        for (unsigned int k = 0; k < getBufferSize(); ++k) {
            seed[i] *= 282475249;
            Sample white = static_cast<Sample>(seed[i]) * 4.65661287e-10;
#if DSP_USE_VC
            Vector sum = 0.0;
            for (std::size_t m = 0; m < b.vectorsCount(); ++m) {
                const Vector value = valueCoefficients.vector(m);
                const Vector noise = noiseCoefficients.vector(m);
                b.vector(m) = b.vector(m) * value + white * noise;
                sum += b.vector(m);
            }
            outputBuffer[k] = 0.125 * sum.sum();
#else
            b[0] = b[0] * VALUE_COEFFICIENT_0 + white * NOISE_COEFFICIENT_0;
            b[1] = b[1] * VALUE_COEFFICIENT_1 + white * NOISE_COEFFICIENT_1;
            b[2] = b[2] * VALUE_COEFFICIENT_2 + white * NOISE_COEFFICIENT_2;
            b[3] = b[3] * VALUE_COEFFICIENT_3 + white * NOISE_COEFFICIENT_3;
            b[4] = b[4] * VALUE_COEFFICIENT_4 + white * NOISE_COEFFICIENT_4;
            b[5] = b[5] * VALUE_COEFFICIENT_5 + white * NOISE_COEFFICIENT_5;
            b[6] = white * NOISE_COEFFICIENT_6;
            Sample sum = 0.0;
            for (const auto &value : b) {
                sum += value;
            }
            outputBuffer[k] = 0.125 * sum;
#endif
            b[7] = white * NOISE_COEFFICIENT_7;
        }
    }
}
