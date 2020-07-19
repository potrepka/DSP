#include "PolarToCartesian.h"

dsp::PolarToCartesian::PolarToCartesian()
        : magnitude(pushInput(Type::UNIPOLAR, Space::FREQUENCY))
        , phase(pushInput(Type::UNIPOLAR, Space::FREQUENCY))
        , real(pushOutput(Type::BIPOLAR, Space::FREQUENCY))
        , imaginary(pushOutput(Type::BIPOLAR, Space::FREQUENCY)) {}

std::shared_ptr<dsp::InputParameter> dsp::PolarToCartesian::getMagnitude() const {
    return magnitude;
}

std::shared_ptr<dsp::InputParameter> dsp::PolarToCartesian::getPhase() const {
    return phase;
}

std::shared_ptr<dsp::OutputParameter> dsp::PolarToCartesian::getReal() const {
    return real;
}

std::shared_ptr<dsp::OutputParameter> dsp::PolarToCartesian::getImaginary() const {
    return imaginary;
}

void dsp::PolarToCartesian::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        Array &magnitudeBuffer = getMagnitude()->getChannel(i)->getBuffer();
        Array &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
        Array &realBuffer = getReal()->getChannel(i)->getBuffer();
        Array &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            Sample theta = TAU * phaseBuffer[k];
            realBuffer[k] = magnitudeBuffer[k] * cos(theta);
            imaginaryBuffer[k] = magnitudeBuffer[k] * sin(theta);
        }
    }
}
