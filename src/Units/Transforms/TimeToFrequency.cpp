#include "TimeToFrequency.h"

dsp::TimeToFrequency::TimeToFrequency()
        : Consumer(Type::BIPOLAR)
        , real(pushOutput(Type::BIPOLAR, Space::FREQUENCY))
        , imaginary(pushOutput(Type::BIPOLAR, Space::FREQUENCY)) {}

std::shared_ptr<dsp::OutputParameter> dsp::TimeToFrequency::getReal() const {
    return real;
}

std::shared_ptr<dsp::OutputParameter> dsp::TimeToFrequency::getImaginary() const {
    return imaginary;
}

void dsp::TimeToFrequency::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    fft.setup(bufferSize);
}

void dsp::TimeToFrequency::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<Sample> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<Sample> &realBuffer = getReal()->getChannel(i)->getBuffer();
        std::vector<Sample> &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();

        fft.fft(inputBuffer, realBuffer, imaginaryBuffer);
    }
}
