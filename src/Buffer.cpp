#include "Buffer.h"

dsp::Buffer::Buffer(unsigned int numChannels, unsigned int bufferSize, Type type, Space space, DSP_FLOAT defaultValue)
        : bufferSize(bufferSize)
        , type(type)
        , space(space)
        , defaultValue(defaultValue) {
    setNumChannels(numChannels);
    setBufferSize(bufferSize);
}

unsigned int dsp::Buffer::getNumChannels() const {
    return static_cast<unsigned int>(buffers.size());
}

void dsp::Buffer::setNumChannels(unsigned int numChannels) {
    lock();
    setNumChannelsNoLock(numChannels);
    unlock();
}

void dsp::Buffer::setNumChannelsNoLock(unsigned int numChannels) {
    if (numChannels < buffers.size()) {
        buffers.erase(buffers.begin() + numChannels, buffers.end());
    } else {
        buffers.reserve(numChannels);
        for (unsigned int i = getNumChannels(); i < numChannels; i++) {
            buffers.push_back(std::vector<DSP_FLOAT>(bufferSize, defaultValue));
        }
    }
}

unsigned int dsp::Buffer::getBufferSize() const {
    return bufferSize;
}

void dsp::Buffer::setBufferSize(unsigned int bufferSize) {
    lock();
    setBufferSizeNoLock(bufferSize);
    unlock();
}

void dsp::Buffer::setBufferSizeNoLock(unsigned int bufferSize) {
    this->bufferSize = bufferSize;
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        buffers[i].resize(bufferSize, defaultValue);
    }
}

dsp::Type dsp::Buffer::getType() const {
    return type;
}

void dsp::Buffer::setType(Type type) {
    this->type = type;
}

dsp::Space dsp::Buffer::getSpace() const {
    return space;
}

void dsp::Buffer::setSpace(Space space) {
    this->space = space;
}

DSP_FLOAT dsp::Buffer::getDefaultValue() const {
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
    fillBufferNoLock(value);
    unlock();
}

void dsp::Buffer::fillBufferNoLock(DSP_FLOAT value) {
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        std::fill(buffers[i].begin(), buffers[i].end(), value);
    }
}

void dsp::Buffer::insert(unsigned int start, std::shared_ptr<Buffer> buffer) {
    lock();
    if (start > getBufferSize()) {
        start = getBufferSize();
    }
    for (unsigned int i = 0; i < getNumChannels() && i < buffer->getNumChannels(); i++) {
        buffers[i].insert(buffers[i].begin() + start, buffer->getChannel(i).begin(), buffer->getChannel(i).end());
    }
    unlock();
}

std::shared_ptr<dsp::Buffer> dsp::Buffer::clip(unsigned int start, unsigned int end) {
    std::shared_ptr<dsp::Buffer> buffer;
    lock();
    if (end > getBufferSize()) {
        end = getBufferSize();
    }
    if (start > end) {
        start = end;
    }
    buffer = std::make_shared<Buffer>(getNumChannels(), end - start, type, space, defaultValue);
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        for (unsigned int k = start; k < end; k++) {
            buffer->getChannel(i)[k] = buffers[i][k];
        }
    }
    unlock();
    return buffer;
}
