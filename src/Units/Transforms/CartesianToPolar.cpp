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
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &realBuffer = getReal()->getChannel(i)->getBuffer();
        Array &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();
        Array &magnitudeBuffer = getMagnitude()->getChannel(i)->getBuffer();
        Array &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
        Iterator realIterator = realBuffer.begin();
        Iterator imaginaryIterator = imaginaryBuffer.begin();
        Iterator magnitudeIterator = magnitudeBuffer.begin();
        Iterator phaseIterator = phaseBuffer.begin();
        while (realIterator != realBuffer.end()) {
#if DSP_USE_VC
            Vector re = *realIterator;
            Vector im = *imaginaryIterator;
            *magnitudeIterator = Vc::sqrt(re * re + im * im);
            Vector bipolar = ONE_OVER_TAU * Vc::atan2(im, re);
            *phaseIterator = Vc::iif(bipolar < Vector::Zero(), bipolar + Vector::One(), bipolar);
#else
            Sample re = *realIterator;
            Sample im = *imaginaryIterator;
            *magnitudeIterator = sqrt(re * re + im * im);
            Sample bipolar = ONE_OVER_TAU * atan2(im, re);
            *phaseIterator = bipolar < 0.0 ? bipolar + 1.0 : bipolar;
#endif
            ++realIterator;
            ++imaginaryIterator;
            ++magnitudeIterator;
            ++phaseIterator;
        }
    }
}
