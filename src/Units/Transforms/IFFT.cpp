#include "IFFT.h"

const unsigned int dsp::IFFT::REAL = 0;
const unsigned int dsp::IFFT::IMAGINARY = 1;

dsp::IFFT::IFFT() : Generator(Type::BIPOLAR) {
    pushInput(Type::BIPOLAR, Space::FREQUENCY);
    pushInput(Type::BIPOLAR, Space::FREQUENCY);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::IFFT::getReal() const {
    return getInput(REAL);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::IFFT::getImaginary() const {
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
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &realBuffer = getReal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &imaginaryBuffer = getImaginary()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();

        std::copy(realBuffer.begin(), realBuffer.end(), real.begin());
        std::copy(imaginaryBuffer.begin(), imaginaryBuffer.end(), imaginary.begin());
        fft.ifft(output.data(), real.data(), imaginary.data());
        std::transform(output.begin(), output.end(), outputBuffer.begin(), [this](DSP_FLOAT x) {
            return x * getBufferSize();
        });
    }
}
