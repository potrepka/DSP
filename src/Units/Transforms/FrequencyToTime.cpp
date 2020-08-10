#include "FrequencyToTime.h"

dsp::FrequencyToTime::FrequencyToTime(Type type)
        : Generator(type)
        , real(pushInput(Type::BIPOLAR, Space::FREQUENCY))
        , imaginary(pushInput(Type::BIPOLAR, Space::FREQUENCY)) {}

std::shared_ptr<dsp::InputParameter> dsp::FrequencyToTime::getReal() const {
    return real;
}

std::shared_ptr<dsp::InputParameter> dsp::FrequencyToTime::getImaginary() const {
    return imaginary;
}

void dsp::FrequencyToTime::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    fft.setup(bufferSize);
}

void dsp::FrequencyToTime::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        Array &realBuffer = getReal()->getChannel(i)->getBuffer();
        Array &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();
        Array &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();

        fft.ifft(outputBuffer, realBuffer, imaginaryBuffer);
    }
}
