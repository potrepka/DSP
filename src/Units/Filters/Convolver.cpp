#include "Convolver.h"

dsp::Convolver::Convolver()
        : Processor(Type::BIPOLAR, Type::BIPOLAR)
        , headSize(0)
        , tailSize(0) {}

std::shared_ptr<dsp::Buffer> dsp::Convolver::getBuffer(unsigned int channel) const {
    assert(channel < buffers.size());
    return buffers[channel];
}

void dsp::Convolver::setBuffer(unsigned int channel, std::shared_ptr<dsp::Buffer> buffer) {
    assert(channel < buffers.size());
    buffers[channel] = buffer;
    lock();
    initConvolver(channel);
    unlock();
}

unsigned int dsp::Convolver::getHeadSize() const {
    return headSize;
}

unsigned int dsp::Convolver::getTailSize() const {
    return tailSize;
}

void dsp::Convolver::setHeadSize(unsigned int size) {
    headSize = size;
    initConvolvers();
}

void dsp::Convolver::setTailSize(unsigned int size) {
    tailSize = size;
    initConvolvers();
}

void dsp::Convolver::initConvolvers() {
    lock();
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
    buffers.resize(numChannels);
    convolvers.resize(numChannels);
}

void dsp::Convolver::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        Array &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        for (unsigned int j = 0; j < convolvers[i].size(); j++) {
            Array &outputBuffer = getOutputSignal()->getChannel(j % getNumChannels())->getBuffer();
            if (convolvers[i][j] != nullptr) {
                std::copy(inputBuffer.begin(), inputBuffer.end(), input.begin());
                convolvers[i][j]->process(input.data(), output.data(), getBufferSize());
                std::transform(
                        output.begin(), output.end(), outputBuffer.begin(), outputBuffer.begin(), std::plus<Sample>());
            }
        }
    }
}

void dsp::Convolver::initConvolver(unsigned int channel) {
    convolvers[channel].clear();
    buffers[channel]->lock();
    if (buffers[channel] != nullptr) {
        unsigned int numChannels = buffers[channel]->getNumChannels();
        unsigned int bufferSize = buffers[channel]->getBufferSize();
        std::vector<std::vector<fftconvolver::Sample>> sample(numChannels,
                                                              std::vector<fftconvolver::Sample>(bufferSize));
        for (unsigned int j = 0; j < numChannels; j++) {
            Array &buffer = buffers[channel]->getChannel(j);
            std::copy(buffer.begin(), buffer.end(), sample[j].begin());
            convolvers[channel].emplace_back(std::make_unique<fftconvolver::TwoStageFFTConvolver>());
            convolvers[channel][j]->init(headSize, tailSize, sample[j].data(), sample[j].size());
        }
    }
    buffers[channel]->unlock();
}
