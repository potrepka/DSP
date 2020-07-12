#include "BufferUnit.h"

dsp::BufferUnit::BufferUnit(Type type, Space space, DSP_FLOAT defaultValue)
        : Consumer(type, space), mode(Mode::SINGLE_BUFFER), externalBufferSize(0), writeIndex(0) {
    buffer = std::make_shared<Buffer>(0, 0, type, space, defaultValue);
    second = std::make_shared<Buffer>(0, 0, type, space, defaultValue);
}

dsp::BufferUnit::Mode dsp::BufferUnit::getMode() const {
    return mode;
}

void dsp::BufferUnit::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

unsigned int dsp::BufferUnit::getExternalBufferSize() const {
    return externalBufferSize;
}

void dsp::BufferUnit::setExternalBufferSize(unsigned int externalBufferSize) {
    lock();
    this->externalBufferSize = externalBufferSize;
    buffer->setBufferSize(externalBufferSize);
    second->setBufferSize(externalBufferSize);
    unlock();
}

std::shared_ptr<dsp::Buffer> dsp::BufferUnit::getBuffer() const {
    return buffer;
}

void dsp::BufferUnit::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    buffer->setNumChannels(numChannels);
    second->setNumChannels(numChannels);
}

void dsp::BufferUnit::process() {
    Unit::process();
    buffer->lock();
    if (externalBufferSize > 0) {
        if (getInputSignal()->getSpace() == Space::FREQUENCY) {
            assert(getBufferSize() == externalBufferSize);
        }
        if (writeIndex >= externalBufferSize || getBufferSize() == externalBufferSize) {
            writeIndex = 0;
        }
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
            unsigned int index = writeIndex;
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                switch (mode) {
                    case Mode::SINGLE_BUFFER: buffer->getChannel(i)[index] = inputBuffer[k]; break;
                    case Mode::DOUBLE_BUFFER: second->getChannel(i)[index] = inputBuffer[k]; break;
                }
                index++;
                if (index >= externalBufferSize) {
                    index = 0;
                    if (mode == Mode::DOUBLE_BUFFER) {
                        std::copy(second->getChannel(i).begin(),
                                  second->getChannel(i).end(),
                                  buffer->getChannel(i).begin());
                    }
                }
            }
        }
        if (getBufferSize() != externalBufferSize) {
            writeIndex += getBufferSize();
            writeIndex %= externalBufferSize;
        }
    }
    buffer->unlock();
}
