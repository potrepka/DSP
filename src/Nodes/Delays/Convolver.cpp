#include "Convolver.h"

dsp::Convolver::Convolver()
        : Transformer(Type::RATIO, Type::RATIO)
        , headSize(0)
        , tailSize(0) {}

int dsp::Convolver::getHeadSize() const {
    return headSize;
}

void dsp::Convolver::setHeadSize(int headSize) {
    this->headSize = headSize;
    initConvolvers();
}

int dsp::Convolver::getTailSize() const {
    return tailSize;
}

void dsp::Convolver::setTailSize(int tailSize) {
    this->tailSize = tailSize;
    initConvolvers();
}

std::shared_ptr<dsp::Buffer> dsp::Convolver::getBuffer(int inChannel, int outChannel) const {
    DSP_ASSERT(inChannel >= 0 && inChannel < buffers.size());
    DSP_ASSERT(outChannel >= 0 && outChannel < buffers[inChannel].size());
    return buffers[inChannel][outChannel];
}

void dsp::Convolver::setBuffer(int inChannel, int outChannel, std::shared_ptr<dsp::Buffer> buffer) {
    DSP_ASSERT(inChannel >= 0 && inChannel < buffers.size());
    DSP_ASSERT(outChannel >= 0 && outChannel < buffers[inChannel].size());
    DSP_ASSERT(buffer->getNumChannels() == 1);
    buffers[inChannel][outChannel] = buffer;
    lock();
    initConvolver(inChannel, outChannel);
    unlock();
}

void dsp::Convolver::initConvolvers() {
    lock();
    for (int inChannel = 0; inChannel < getNumInputChannels(); ++inChannel) {
        for (int outChannel = 0; outChannel < getNumInputChannels(); ++outChannel) {
            initConvolver(inChannel, outChannel);
        }
    }
    unlock();
}

void dsp::Convolver::setNumInputChannelsNoLock(int numChannels) {
    Node::setNumInputChannelsNoLock(numChannels);
    buffers.resize(numChannels);
    convolvers.resize(numChannels);
}

void dsp::Convolver::setNumOutputChannelsNoLock(int numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    for (int inChannel = 0; inChannel < getNumInputChannels(); ++inChannel) {
        buffers[inChannel].resize(numChannels);
        convolvers[inChannel].resize(numChannels);
    }
}

void dsp::Convolver::setNumSamplesNoLock(int numSamples) {
    Node::setNumSamplesNoLock(numSamples);
    input.resize(numSamples);
    output.resize(numSamples);
}

void dsp::Convolver::processNoLock() {
    for (int inChannel = 0; inChannel < getNumInputChannels(); ++inChannel) {
        Sample *inputChannel = getInput()->getBlock().getChannelPointer(inChannel);
        for (int outChannel = 0; outChannel < getNumOutputChannels(); ++outChannel) {
            Sample *outputChannel = getOutput()->getBlock().getChannelPointer(outChannel);
            if (convolvers[inChannel][outChannel] != nullptr) {
                for (int sample = 0; sample < getNumSamples(); ++sample) {
                    input[sample] = inputChannel[sample];
                }
                convolvers[inChannel][outChannel]->process(input.data(), output.data(), getNumSamples());
                for (int sample = 0; sample < getNumSamples(); ++sample) {
                    outputChannel[sample] += output[sample];
                }
            }
        }
    }
}

void dsp::Convolver::initConvolver(int inChannel, int outChannel) {
    DSP_ASSERT(inChannel >= 0 && inChannel < convolvers.size());
    DSP_ASSERT(outChannel >= 0 && outChannel < convolvers[inChannel].size());
    if (buffers[inChannel][outChannel] != nullptr) {
        buffers[inChannel][outChannel]->lock();
        int numSamples = buffers[inChannel][outChannel]->getNumSamples();
        std::vector<fftconvolver::Sample> buffer(numSamples);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            Sample *channel = buffers[inChannel][outChannel]->getBlock().getChannelPointer(0);
            buffer[sample] = channel[sample];
        }
        convolvers[inChannel][outChannel] = std::make_unique<fftconvolver::TwoStageFFTConvolver>();
        convolvers[inChannel][outChannel]->init(headSize, tailSize, buffer.data(), buffer.size());
        buffers[inChannel][outChannel]->unlock();
    }
}
