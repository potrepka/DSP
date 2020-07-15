#include "FFT.h"

dsp::FFT::FFT()
        : Consumer(Type::BIPOLAR)
        , real(pushOutput(Type::BIPOLAR, Space::FREQUENCY))
        , imaginary(pushOutput(Type::BIPOLAR, Space::FREQUENCY)) {}

std::shared_ptr<dsp::OutputParameter> dsp::FFT::getReal() const {
    return real;
}

std::shared_ptr<dsp::OutputParameter> dsp::FFT::getImaginary() const {
    return imaginary;
}

void dsp::FFT::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    fft.init(bufferSize);
    inputFloats.resize(bufferSize);
    realFloats.resize(audiofft::AudioFFT::ComplexSize(bufferSize));
    imaginaryFloats.resize(audiofft::AudioFFT::ComplexSize(bufferSize));
}

void dsp::FFT::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &realBuffer = getReal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();

        std::transform(inputBuffer.begin(), inputBuffer.end(), inputFloats.begin(), [this](DSP_FLOAT x) {
            return x * getOneOverBufferSize();
        });
        fft.fft(inputFloats.data(), realFloats.data(), imaginaryFloats.data());
        std::copy(realFloats.begin(), realFloats.end(), realBuffer.begin());
        std::copy(imaginaryFloats.begin(), imaginaryFloats.end(), imaginaryBuffer.begin());
    }
}
