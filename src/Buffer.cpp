#include "Buffer.h"

dsp::Buffer::Buffer(unsigned int numChannels, unsigned int bufferSize, Type type, Space space, DSP_FLOAT defaultValue)
        : bufferSize(bufferSize), type(type), space(space), defaultValue(defaultValue) {
    setNumChannels(numChannels);
    setBufferSize(bufferSize);
}

unsigned int dsp::Buffer::getNumChannels() {
    return static_cast<unsigned int>(buffers.size());
}

void dsp::Buffer::setNumChannels(unsigned int numChannels) {
    lock();
    if (numChannels < buffers.size()) {
        buffers.erase(buffers.begin() + numChannels, buffers.end());
    } else {
        buffers.reserve(numChannels);
        for (unsigned int i = getNumChannels(); i < numChannels; i++) {
            buffers.push_back(std::vector<DSP_FLOAT>(bufferSize, defaultValue));
        }
    }
    unlock();
}

unsigned int dsp::Buffer::getBufferSize() {
    return bufferSize;
}

void dsp::Buffer::setBufferSize(unsigned int bufferSize) {
    lock();
    this->bufferSize = bufferSize;
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        buffers[i].resize(bufferSize);
    }
    unlock();
}

dsp::Type dsp::Buffer::getType() {
    return type;
}

void dsp::Buffer::setType(Type type) {
    this->type = type;
}

dsp::Space dsp::Buffer::getSpace() {
    return space;
}

void dsp::Buffer::setSpace(Space space) {
    this->space = space;
}

DSP_FLOAT dsp::Buffer::getDefaultValue() {
    return defaultValue;
}

void dsp::Buffer::setDefaultValue(DSP_FLOAT defaultValue) {
    this->defaultValue = defaultValue;
}

std::vector<DSP_FLOAT> &dsp::Buffer::getChannel(unsigned int channel) {
    return buffers[channel];
}

void dsp::Buffer::fillBuffer(DSP_FLOAT value) {
    lock();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::fill(buffers[i].begin(), buffers[i].end(), value);
    }
    unlock();
}
