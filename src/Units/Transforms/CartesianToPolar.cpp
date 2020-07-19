#include "CartesianToPolar.h"

dsp::CartesianToPolar::CartesianToPolar()
        : real(pushInput(Type::BIPOLAR, Space::FREQUENCY))
        , imaginary(pushInput(Type::BIPOLAR, Space::FREQUENCY))
        , magnitude(pushOutput(Type::UNIPOLAR, Space::FREQUENCY))
        , phase(pushOutput(Type::UNIPOLAR, Space::FREQUENCY)) {}

std::shared_ptr<dsp::InputParameter> dsp::CartesianToPolar::getReal() const {
    return real;
}

std::shared_ptr<dsp::InputParameter> dsp::CartesianToPolar::getImaginary() const {
    return imaginary;
}

std::shared_ptr<dsp::OutputParameter> dsp::CartesianToPolar::getMagnitude() const {
    return magnitude;
}

std::shared_ptr<dsp::OutputParameter> dsp::CartesianToPolar::getPhase() const {
    return phase;
}

void dsp::CartesianToPolar::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        Array &realBuffer = getReal()->getChannel(i)->getBuffer();
        Array &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();
        Array &magnitudeBuffer = getMagnitude()->getChannel(i)->getBuffer();
        Array &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            magnitudeBuffer[k] = sqrt(realBuffer[k] * realBuffer[k] + imaginaryBuffer[k] * imaginaryBuffer[k]);
            Sample bipolar = ONE_OVER_TAU * atan2(imaginaryBuffer[k], realBuffer[k]);
            phaseBuffer[k] = bipolar < 0.0 ? bipolar + 1.0 : bipolar;
        }
    }
}
