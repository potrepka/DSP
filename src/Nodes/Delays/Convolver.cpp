#include "Convolver.h"

dsp::Convolver::Convolver()
        : Transformer(Type::RATIO, Type::RATIO)
        , headSize(0)
        , tailSize(0) {}

size_t dsp::Convolver::getHeadSize() const {
    return headSize;
}

void dsp::Convolver::setHeadSize(size_t headSize) {
    this->headSize = headSize;
    initConvolvers();
}

size_t dsp::Convolver::getTailSize() const {
    return tailSize;
}

void dsp::Convolver::setTailSize(size_t tailSize) {
    this->tailSize = tailSize;
    initConvolvers();
}

std::shared_ptr<dsp::Buffer> dsp::Convolver::getBuffer(size_t inChannel, size_t outChannel) const {
    DSP_ASSERT(inChannel < buffers.size() && outChannel < buffers[inChannel].size());
    return buffers[inChannel][outChannel];
}

void dsp::Convolver::setBuffer(size_t inChannel, size_t outChannel, std::shared_ptr<dsp::Buffer> buffer) {
    DSP_ASSERT(inChannel < buffers.size() && outChannel < buffers[inChannel].size() && buffer->getNumChannels() == 1);
    buffers[inChannel][outChannel] = buffer;
    lock();
    initConvolver(inChannel, outChannel);
    unlock();
}

void dsp::Convolver::initConvolvers() {
    lock();
    for (size_t inChannel = 0; inChannel < getNumInputChannels(); ++inChannel) {
        for (size_t outChannel = 0; outChannel < getNumInputChannels(); ++outChannel) {
            initConvolver(inChannel, outChannel);
        }
    }
    unlock();
}

void dsp::Convolver::setNumInputChannelsNoLock(size_t numChannels) {
    Node::setNumInputChannelsNoLock(numChannels);
    buffers.resize(numChannels);
    convolvers.resize(numChannels);
}

void dsp::Convolver::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    for (size_t inChannel = 0; inChannel < getNumInputChannels(); ++inChannel) {
        buffers[inChannel].resize(numChannels);
        convolvers[inChannel].resize(numChannels);
    }
}

void dsp::Convolver::setNumSamplesNoLock(size_t numSamples) {
    Node::setNumSamplesNoLock(numSamples);
    input.resize(numSamples);
    output.resize(numSamples);
}

void dsp::Convolver::processNoLock() {
    for (size_t inChannel = 0; inChannel < getNumInputChannels(); ++inChannel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(inChannel);
        for (size_t outChannel = 0; outChannel < getNumOutputChannels(); ++outChannel) {
            Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(outChannel);
            if (convolvers[inChannel][outChannel] != nullptr) {
                for (size_t sample = 0; sample < getNumSamples(); ++sample) {
                    input[sample] = inputChannel[sample];
                }
                convolvers[inChannel][outChannel]->process(input.data(), output.data(), getNumSamples());
                for (size_t sample = 0; sample < getNumSamples(); ++sample) {
                    outputChannel[sample] += output[sample];
                }
            }
        }
    }
}

void dsp::Convolver::initConvolver(size_t inChannel, size_t outChannel) {
    DSP_ASSERT(inChannel < convolvers.size() && outChannel < convolvers[inChannel].size());
    if (buffers[inChannel][outChannel] != nullptr) {
        buffers[inChannel][outChannel]->lock();
        size_t numSamples = buffers[inChannel][outChannel]->getNumSamples();
        std::vector<fftconvolver::Sample> buffer(numSamples);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample *channel = buffers[inChannel][outChannel]->getWrapper().getChannelPointer(0);
            buffer[sample] = channel[sample];
        }
        convolvers[inChannel][outChannel] = std::make_unique<fftconvolver::TwoStageFFTConvolver>();
        convolvers[inChannel][outChannel]->init(headSize, tailSize, buffer.data(), buffer.size());
        buffers[inChannel][outChannel]->unlock();
    }
}
