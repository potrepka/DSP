#include "BiquadBPF.h"

void dsp::BiquadBPF::calculateCoefficients(const DSP_FLOAT& frequency, const DSP_FLOAT& q, const DSP_FLOAT& gain) {
    double omega = TAU * frequency / getSampleRate();
    double sinW = sin(omega);
    double cosW = cos(omega);
    double alpha = sinW / (2.0 * q);

    a0 = 1.0 + alpha;
    a1 = -2.0 * cosW;
    a2 = 1.0 - alpha;

    b0 = alpha;
    b1 = 0.0;
    b2 = -alpha;
}
