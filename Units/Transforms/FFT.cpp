#include "FFT.h"

const std::size_t dsp::FFT::REAL = 0;
const std::size_t dsp::FFT::IMAGINARY = 1;

dsp::FFT::FFT() : Consumer(Connection::Type::BIPOLAR), inverted(false) {
    pushOutput(Connection::Type::FFT_REAL);
    pushOutput(Connection::Type::FFT_IMAGINARY);
}

void dsp::FFT::setBufferSize(unsigned int bufferSize) {
    lock();
    setBufferSizeNoLock(bufferSize);
    fft.init(bufferSize);
    input.resize(bufferSize);
    real.resize(audiofft::AudioFFT::ComplexSize(bufferSize));
    imaginary.resize(audiofft::AudioFFT::ComplexSize(bufferSize));
    unlock();
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::FFT::getReal() {
    return getOutput(REAL);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::FFT::getImaginary() {
    return getOutput(IMAGINARY);
}

void dsp::FFT::process() {
    Unit::process();
    for (int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &realBuffer = getReal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();
        std::copy(inputBuffer.begin(), inputBuffer.end(), input.begin());
        if (inverted) {
            fft.ifft(input.data(), real.data(), imaginary.data());
        } else {
            fft.fft(input.data(), real.data(), imaginary.data());
        }
        std::copy(real.begin(), real.end(), realBuffer.begin());
        std::copy(imaginary.begin(), imaginary.end(), imaginaryBuffer.begin());
    }
}
