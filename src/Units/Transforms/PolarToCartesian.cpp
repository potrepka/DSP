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
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &magnitudeBuffer = getMagnitude()->getChannel(i)->getBuffer();
        Array &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
        Array &realBuffer = getReal()->getChannel(i)->getBuffer();
        Array &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();
        Iterator realIterator = realBuffer.begin();
        Iterator imaginaryIterator = imaginaryBuffer.begin();
        Iterator magnitudeIterator = magnitudeBuffer.begin();
        Iterator phaseIterator = phaseBuffer.begin();
        while (magnitudeIterator != magnitudeBuffer.end()) {
#if DSP_USE_VC
            Vector theta = TAU * *phaseIterator;
            *realIterator = *magnitudeIterator * Vc::cos(theta);
            *imaginaryIterator = *magnitudeIterator * Vc::sin(theta);
#else
            Sample theta = TAU * *phaseIterator;
            *realIterator = *magnitudeIterator * cos(theta);
            *imaginaryIterator = *magnitudeIterator * sin(theta);
#endif
            ++realIterator;
            ++imaginaryIterator;
            ++magnitudeIterator;
            ++phaseIterator;
        }
    }
}
