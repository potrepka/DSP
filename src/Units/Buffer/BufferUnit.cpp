#include "BufferUnit.h"

dsp::BufferUnit::BufferUnit() : Consumer(Type::BIPOLAR), index(0) {
    buffer = std::make_shared<Buffer>(0, 0, Type::BIPOLAR);
}

std::shared_ptr<dsp::Buffer> dsp::BufferUnit::getBuffer() {
    return buffer;
}

void dsp::BufferUnit::setBuffer(std::shared_ptr<Buffer> buffer) {
    assert(buffer != nullptr);
    lock();
    buffer->setNumChannels(getNumChannels());
    this->buffer = buffer;
    unlock();
}

void dsp::BufferUnit::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    buffer->setNumChannels(numChannels);
}

void dsp::BufferUnit::process() {
    Unit::process();
    buffer->lock();
    if (buffer->getBufferSize() > 0) {
        index %= buffer->getBufferSize();
        unsigned int start = index;
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
            unsigned int position = start;
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                buffer->getChannel(i)[position] = inputBuffer[k];
                position++;
                if (position >= buffer->getBufferSize()) {
                    position -= buffer->getBufferSize();
                }
            }
        }
        index += getBufferSize();
    }
    buffer->unlock();
}
