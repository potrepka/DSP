#include "Convolver.h"

dsp::Convolver::Convolver()
        : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR), headSize(0), tailSize(0) {}

std::vector<DSP_FLOAT> dsp::Convolver::getSample(unsigned int channel) {
    return samples[channel];
}

void dsp::Convolver::setSample(unsigned int channel, const std::vector<DSP_FLOAT> &sample) {
    lock();
    samples[channel] = sample;
    initConvolver(channel);
    unlock();
}

unsigned int dsp::Convolver::getHeadSize() {
    return headSize;
}

unsigned int dsp::Convolver::getTailSize() {
    return tailSize;
}

void dsp::Convolver::setHeadSize(unsigned int size) {
    lock();
    headSize = size;
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        initConvolver(i);
    }
    unlock();
}

void dsp::Convolver::setTailSize(unsigned int size) {
    lock();
    tailSize = size;
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        initConvolver(i);
    }
    unlock();
}

void dsp::Convolver::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    input.resize(bufferSize);
    output.resize(bufferSize);
}

void dsp::Convolver::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    samples.resize(numChannels);
    unsigned int currentSize = static_cast<unsigned int>(convolvers.size());
    if (numChannels < currentSize) {
        convolvers.resize(numChannels);
    } else {
        convolvers.reserve(numChannels);
        for (unsigned int i = currentSize; i < numChannels; i++) {
            convolvers.emplace_back(std::make_unique<fftconvolver::TwoStageFFTConvolver>());
        }
    }
}

void dsp::Convolver::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
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

void dsp::Convolver::initConvolver(unsigned int channel) {
    std::vector<fftconvolver::Sample> sample(samples[channel].size());
    std::copy(samples[channel].begin(), samples[channel].end(), sample.begin());
    convolvers[channel]->init(headSize, tailSize, sample.data(), sample.size());
}
