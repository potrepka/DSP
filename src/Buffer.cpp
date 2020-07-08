#include "Buffer.h"

template <dsp::Type T, dsp::Space S> dsp::Buffer<T, S>::Buffer(unsigned int size) : size(size) {}

template <dsp::Type T, dsp::Space S> unsigned int dsp::Buffer<T, S>::getNumChannels() {
    return static_cast<unsigned int>(buffers.size());
}

template <dsp::Type T, dsp::Space S> void dsp::Buffer<T, S>::setNumChannels(unsigned int numChannels) {
    lock();
    if (numChannels < buffers.size()) {
        buffers.erase(buffers.begin() + numChannels, buffers.end());
    } else {
        buffers.reserve(numChannels);
        for (unsigned int i = static_cast<unsigned int>(buffers.size()); i < numChannels; i++) {
            buffers.push_back(std::vector<DSP_FLOAT>(size, 0.0));
        }
    }
    unlock();
}

template <dsp::Type T, dsp::Space S> unsigned int dsp::Buffer<T, S>::getSize() {
    return size;
}

template <dsp::Type T, dsp::Space S> void dsp::Buffer<T, S>::setSize(unsigned int size) {
    lock();
    this->size = size;
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        buffers[i].resize(size);
    }
    unlock();
}

template <dsp::Type T, dsp::Space S> std::vector<DSP_FLOAT> &dsp::Buffer<T, S>::getBuffer(unsigned int channel) {
    return buffers[channel];
}
