#include "FrequencyToTime.h"

dsp::FrequencyToTime::FrequencyToTime()
        : Generator(Type::BIPOLAR)
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
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &realBuffer = getReal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();

        fft.ifft(outputBuffer, realBuffer, imaginaryBuffer);
    }
}
