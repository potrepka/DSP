#include "BiquadPeak.h"

void dsp::BiquadPeak::calculateCoefficients(const DSP_FLOAT &frequency, const DSP_FLOAT &q, const DSP_FLOAT &gain) {
    double omega = TAU * frequency / getSampleRate();
    double sinW = sin(omega);
    double cosW = cos(omega);
    double alpha = sinW / (2.0 * q);

    double amp = pow(10.0, gain * 0.025);
    double alphaMore = alpha * amp;
    double alphaLess = alpha / amp;

    a0 = 1.0 + alphaLess;
    a1 = -2.0 * cosW;
    a2 = 1.0 - alphaLess;

    b0 = 1.0 + alphaMore;
    b1 = a1;
    b2 = 1.0 - alphaMore;
}
