#include "PolarToCartesian.h"

dsp::PolarToCartesian::PolarToCartesian()
        : MAGNITUDE(pushInput(Type::UNIPOLAR, Space::FREQUENCY))
        , PHASE(pushInput(Type::UNIPOLAR, Space::FREQUENCY))
        , REAL(pushOutput(Type::BIPOLAR, Space::FREQUENCY))
        , IMAGINARY(pushOutput(Type::BIPOLAR, Space::FREQUENCY)) {}

std::shared_ptr<dsp::InputParameter> dsp::PolarToCartesian::getMagnitude() const {
    return getInput(MAGNITUDE);
}

std::shared_ptr<dsp::InputParameter> dsp::PolarToCartesian::getPhase() const {
    return getInput(PHASE);
}

std::shared_ptr<dsp::OutputParameter> dsp::PolarToCartesian::getReal() const {
    return getOutput(REAL);
}

std::shared_ptr<dsp::OutputParameter> dsp::PolarToCartesian::getImaginary() const {
    return getOutput(IMAGINARY);
}

void dsp::PolarToCartesian::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &magnitudeBuffer = getMagnitude()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &realBuffer = getReal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();
        for (unsigned int k = 0; k < getBufferSize(); k++) {
            DSP_FLOAT theta = TAU * phaseBuffer[k];
            realBuffer[k] = magnitudeBuffer[k] * cos(theta);
            imaginaryBuffer[k] = magnitudeBuffer[k] * sin(theta);
        }
    }
}
