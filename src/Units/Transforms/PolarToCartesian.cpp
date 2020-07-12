#include "PolarToCartesian.h"

const unsigned int dsp::PolarToCartesian::MAGNITUDE = 0;
const unsigned int dsp::PolarToCartesian::PHASE = 1;
const unsigned int dsp::PolarToCartesian::REAL = 0;
const unsigned int dsp::PolarToCartesian::IMAGINARY = 1;

dsp::PolarToCartesian::PolarToCartesian() {
    pushInput(Type::UNIPOLAR, Space::FREQUENCY);
    pushInput(Type::UNIPOLAR, Space::FREQUENCY);
    pushOutput(Type::BIPOLAR, Space::FREQUENCY);
    pushOutput(Type::BIPOLAR, Space::FREQUENCY);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::PolarToCartesian::getMagnitude() const {
    return getInput(MAGNITUDE);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::PolarToCartesian::getPhase() const {
    return getInput(PHASE);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::PolarToCartesian::getReal() const {
    return getOutput(REAL);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::PolarToCartesian::getImaginary() const {
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
