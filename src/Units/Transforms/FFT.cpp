#include "FFT.h"

const std::size_t dsp::FFT::REAL = 0;
const std::size_t dsp::FFT::IMAGINARY = 1;

dsp::FFT::FFT() : Consumer(Connection::Type::BIPOLAR) {
    pushOutput(Connection::Type::BIPOLAR, Connection::Space::FREQUENCY);
    pushOutput(Connection::Type::BIPOLAR, Connection::Space::FREQUENCY);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::FFT::getReal() {
    return getOutput(REAL);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::FFT::getImaginary() {
    return getOutput(IMAGINARY);
}

void dsp::FFT::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    fft.init(bufferSize);
    input.resize(bufferSize);
    real.resize(audiofft::AudioFFT::ComplexSize(bufferSize));
    imaginary.resize(audiofft::AudioFFT::ComplexSize(bufferSize));
}

void dsp::FFT::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &realBuffer = getReal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();

        std::transform(inputBuffer.begin(), inputBuffer.end(), input.begin(), [this](DSP_FLOAT x) {
            return x * getOneOverBufferSize();
        });
        fft.fft(input.data(), real.data(), imaginary.data());
        std::copy(real.begin(), real.end(), realBuffer.begin());
        std::copy(imaginary.begin(), imaginary.end(), imaginaryBuffer.begin());
    }
}
