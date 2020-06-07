#include "BiquadLowShelf.h"

void dsp::BiquadLowShelf::calculateCoefficients(const DSP_FLOAT& frequency, const DSP_FLOAT& q, const DSP_FLOAT& gain) {
    double omega = TAU * frequency / getSampleRate();
    double sinW = sin(omega);
    double cosW = cos(omega);

    double amp = pow(10.0, gain * 0.025);
    double ampPlus = amp + 1.0;
    double ampMinus = amp - 1.0;
    double betaTimesSinW = sinW * sqrt(amp) / q;
    double ampPlusTimesCosW = ampPlus * cosW;
    double ampMinusTimesCosW = ampMinus * cosW;

    a0 = ampPlus + ampMinusTimesCosW + betaTimesSinW;
    a1 = 2.0 * (ampMinus + ampPlusTimesCosW);
    a2 = ampPlus + ampMinusTimesCosW - betaTimesSinW;

    b0 = amp * (ampPlus - ampMinusTimesCosW + betaTimesSinW);
    b1 = -2.0 * amp * (ampMinus - ampPlusTimesCosW);
    b2 = amp * (ampPlus - ampMinusTimesCosW - betaTimesSinW);
}
