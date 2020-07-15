#include "IFFT.h"

dsp::IFFT::IFFT()
        : Generator(Type::BIPOLAR)
        , real(pushInput(Type::BIPOLAR, Space::FREQUENCY))
        , imaginary(pushInput(Type::BIPOLAR, Space::FREQUENCY)) {}

std::shared_ptr<dsp::InputParameter> dsp::IFFT::getReal() const {
    return real;
}

std::shared_ptr<dsp::InputParameter> dsp::IFFT::getImaginary() const {
    return imaginary;
}

void dsp::IFFT::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    fft.init(bufferSize);
    realFloats.resize(audiofft::AudioFFT::ComplexSize(bufferSize));
    imaginaryFloats.resize(audiofft::AudioFFT::ComplexSize(bufferSize));
    outputFloats.resize(bufferSize);
}

void dsp::IFFT::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &realBuffer = getReal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();

        std::copy(realBuffer.begin(), realBuffer.end(), realFloats.begin());
        std::copy(imaginaryBuffer.begin(), imaginaryBuffer.end(), imaginaryFloats.begin());
        fft.ifft(outputFloats.data(), realFloats.data(), imaginaryFloats.data());
        std::transform(outputFloats.begin(), outputFloats.end(), outputBuffer.begin(), [this](DSP_FLOAT x) {
            return x * getBufferSize();
        });
    }
}
