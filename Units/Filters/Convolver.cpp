#include "Convolver.h"

dsp::Convolver::Convolver()
        : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR), headSize(0), tailSize(0) {}

void dsp::Convolver::setBufferSize(unsigned int bufferSize) {
    lock();
    setBufferSizeNoLock(bufferSize);
    input.resize(bufferSize);
    output.resize(bufferSize);
    unlock();
}

void dsp::Convolver::setNumChannels(std::size_t numChannels) {
    lock();
    setNumChannelsNoLock(numChannels);
    samples.resize(numChannels);
    std::size_t currentSize = convolvers.size();
    if (numChannels < currentSize) {
        convolvers.resize(numChannels);
    } else {
        convolvers.reserve(numChannels);
        for (std::size_t i = currentSize; i < numChannels; i++) {
            convolvers.emplace_back(std::make_unique<fftconvolver::TwoStageFFTConvolver>());
        }
    }
    unlock();
}

std::vector<DSP_FLOAT> dsp::Convolver::getSample(std::size_t index) {
    return samples[index];
}

void dsp::Convolver::setSample(std::size_t index, const std::vector<DSP_FLOAT> &sample) {
    lock();
    samples[index] = sample;
    initConvolver(index);
    unlock();
}

std::size_t dsp::Convolver::getHeadSize() {
    return headSize;
}

std::size_t dsp::Convolver::getTailSize() {
    return tailSize;
}

void dsp::Convolver::setHeadSize(std::size_t size) {
    lock();
    headSize = size;
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        initConvolver(i);
    }
    unlock();
}

void dsp::Convolver::setTailSize(std::size_t size) {
    lock();
    tailSize = size;
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        initConvolver(i);
    }
    unlock();
}

void dsp::Convolver::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
        if (samples[i].size() > 0) {
            std::copy(inputBuffer.begin(), inputBuffer.end(), input.begin());
            convolvers[i]->process(input.data(), output.data(), getBufferSize());
            std::copy(output.begin(), output.end(), outputBuffer.begin());
        } else {
            std::copy(inputBuffer.begin(), inputBuffer.end(), outputBuffer.begin());
        }
    }
}

void dsp::Convolver::initConvolver(std::size_t index) {
    std::vector<fftconvolver::Sample> sample(samples[index].size());
    std::copy(samples[index].begin(), samples[index].end(), sample.begin());
    convolvers[index]->init(headSize, tailSize, sample.data(), sample.size());
}
