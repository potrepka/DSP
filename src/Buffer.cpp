#include "Buffer.h"

dsp::Buffer::Buffer(unsigned int numChannels, unsigned int bufferSize, Type type, Space space, Sample defaultValue)
        : bufferSize(bufferSize)
        , type(type)
        , space(space)
        , defaultValue(defaultValue) {
    setNumChannels(numChannels);
    setBufferSize(bufferSize);
}

dsp::Buffer::Buffer(const Buffer &buffer) {
    buffers = buffer.buffers;
    setNumChannels(buffer.getNumChannels());
    setBufferSize(buffer.getBufferSize());
    setType(buffer.getType());
    setSpace(buffer.getSpace());
    setDefaultValue(buffer.getDefaultValue());
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
        for (unsigned int i = getNumChannels(); i < numChannels; ++i) {
            buffers.push_back(Array(bufferSize, defaultValue));
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
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        buffers[i].resize(bufferSize, defaultValue);
    }
}

dsp::Type dsp::Buffer::getType() const {
    return type;
}

void dsp::Buffer::setType(Type type) {
    lock();
    this->type = type;
    unlock();
}

dsp::Space dsp::Buffer::getSpace() const {
    return space;
}

void dsp::Buffer::setSpace(Space space) {
    lock();
    this->space = space;
    unlock();
}

dsp::Sample dsp::Buffer::getDefaultValue() const {
    return defaultValue;
}

void dsp::Buffer::setDefaultValue(Sample defaultValue) {
    lock();
    this->defaultValue = defaultValue;
    unlock();
}

void dsp::Buffer::fillBuffer(Sample value) {
    lock();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        for (Iterator it = buffers[i].begin(); it != buffers[i].end(); ++it) {
#if DSP_USE_VC
            *it = Vector(value);
#else
            *it = value;
#endif
        }
    }
    unlock();
}

void dsp::Buffer::clearBuffer() {
    lock();
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        for (Iterator it = buffers[i].begin(); it != buffers[i].end(); ++it) {
#if DSP_USE_VC
            *it = Vector(defaultValue);
#else
            *it = defaultValue;
#endif
        }
    }
    unlock();
}

std::vector<dsp::Array> &dsp::Buffer::getChannels() {
    return buffers;
}

dsp::Array &dsp::Buffer::getChannel(unsigned int channel) {
    assert(channel < buffers.size());
    return buffers[channel];
}

dsp::Sample dsp::Buffer::getMinimum(unsigned int channel) const {
    Sample minimum = std::numeric_limits<Sample>::infinity();
    for (const Sample &x : buffers[channel]) {
        if (minimum > x) {
            minimum = x;
        }
    }
    return minimum;
}

dsp::Sample dsp::Buffer::getMaximum(unsigned int channel) const {
    Sample maximum = -std::numeric_limits<Sample>::infinity();
    for (const Sample &x : buffers[channel]) {
        if (maximum < x) {
            maximum = x;
        }
    }
    return maximum;
}

dsp::Sample dsp::Buffer::getMean(unsigned int channel) const {
    if (bufferSize == 0) {
        return defaultValue;
    }
    Sample sum = 0.0;
    for (const Sample &x : buffers[channel]) {
        sum += x;
    }
    return sum / bufferSize;
}

dsp::Sample dsp::Buffer::getRMS(unsigned int channel) const {
    if (bufferSize == 0) {
        return defaultValue;
    }
    Sample sum = 0.0;
    for (const Sample &x : buffers[channel]) {
        sum += x * x;
    }
    return sqrt(sum / bufferSize);
}

void dsp::Buffer::clip(unsigned int begin, unsigned int end) {
    lock();
    if (end > bufferSize) {
        end = bufferSize;
    }
    if (begin > end) {
        begin = end;
    }
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        buffers[i] = Array(buffers[i].begin() + begin, buffers[i].begin() + end);
    }
    bufferSize = end - begin;
    unlock();
}

void dsp::Buffer::stretch(unsigned int bufferSize) {
    lock();
    std::vector<Array> temp = buffers;
    Sample reverseScale = static_cast<Sample>(this->bufferSize) / bufferSize;
    for (unsigned int i = 0; i < getNumChannels(); ++i) {
        buffers[i].resize(bufferSize);
        for (unsigned int k = 0; k < bufferSize; ++k) {
            buffers[i][k] = hermite(temp[i], k * reverseScale);
        }
    }
    this->bufferSize = bufferSize;
    unlock();
}

void dsp::Buffer::insert(unsigned int index, std::shared_ptr<Buffer> buffer) {
    buffer->lock();
    std::vector<Array> temp = buffer->buffers;
    unsigned int bufferSize = buffer->bufferSize;
    buffer->unlock();
    lock();
    if (index > getBufferSize()) {
        index = getBufferSize();
    }
    for (unsigned int i = 0; i < getNumChannels() && i < temp.size(); ++i) {
        buffers[i].insert(buffers[i].begin() + index, temp[i].begin(), temp[i].end());
    }
    this->bufferSize += bufferSize;
    unlock();
}

std::shared_ptr<dsp::Buffer> dsp::Buffer::clone() {
    lock();
    std::shared_ptr<Buffer> buffer = std::make_shared<Buffer>(*this);
    unlock();
    return buffer;
}

unsigned int dsp::BufferCollection::getNumBuffers() const {
    return static_cast<unsigned int>(collection.size());
}

std::shared_ptr<dsp::Buffer> dsp::BufferCollection::getBuffer(unsigned int index) const {
    assert(index < collection.size());
    return collection[index];
}

std::vector<std::shared_ptr<dsp::Buffer>> dsp::BufferCollection::getBuffers(unsigned int begin,
                                                                            unsigned int end) const {
    assert(begin <= end && end <= collection.size());
    return std::vector<std::shared_ptr<dsp::Buffer>>(collection.begin() + begin, collection.begin() + end);
}

void dsp::BufferCollection::pushBuffer(std::shared_ptr<Buffer> buffer) {
    collection.push_back(buffer);
}

void dsp::BufferCollection::pushBuffers(std::vector<std::shared_ptr<Buffer>> buffers) {
    for (const auto &buffer : buffers) {
        collection.push_back(buffer);
    }
}

void dsp::BufferCollection::replaceBuffer(std::shared_ptr<Buffer> buffer, std::shared_ptr<Buffer> replacement) {
    std::replace(collection.begin(), collection.end(), buffer, replacement);
}

void dsp::BufferCollection::removeBuffer(std::shared_ptr<Buffer> buffer) {
    collection.erase(std::remove(collection.begin(), collection.end(), buffer), collection.end());
}

void dsp::BufferCollection::removeBuffers(std::vector<std::shared_ptr<Buffer>> buffers) {
    for (const auto &buffer : buffers) {
        collection.erase(std::remove(collection.begin(), collection.end(), buffer), collection.end());
    }
}
