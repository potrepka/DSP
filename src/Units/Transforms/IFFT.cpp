#include "IFFT.h"

const std::size_t dsp::IFFT::REAL = 0;
const std::size_t dsp::IFFT::IMAGINARY = 1;

dsp::IFFT::IFFT() : Generator(Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::BIPOLAR, Connection::Space::FREQUENCY);
    pushInput(Connection::Type::BIPOLAR, Connection::Space::FREQUENCY);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::IFFT::getReal() {
    return getInput(REAL);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::IFFT::getImaginary() {
    return getInput(IMAGINARY);
}

void dsp::IFFT::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    fft.init(bufferSize);
    real.resize(audiofft::AudioFFT::ComplexSize(bufferSize));
    imaginary.resize(audiofft::AudioFFT::ComplexSize(bufferSize));
    output.resize(bufferSize);
}

void dsp::IFFT::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &realBuffer = getReal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();

        std::copy(realBuffer.begin(), realBuffer.end(), real.begin());
        std::copy(imaginaryBuffer.begin(), imaginaryBuffer.end(), imaginary.begin());
        fft.ifft(output.data(), real.data(), imaginary.data());
        std::copy(output.begin(), output.end(), outputBuffer.begin());
    }
}
