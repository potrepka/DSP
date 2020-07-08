#include "BufferUnit.h"

dsp::BufferUnit::BufferUnit() : Consumer(Type::BIPOLAR), index(0) {}

std::shared_ptr<dsp::Buffer> dsp::BufferUnit::getBuffer() {
    return buffer;
}

void dsp::BufferUnit::setBuffer(std::shared_ptr<Buffer> buffer) {
    this->buffer = buffer;
}

void dsp::BufferUnit::process() {
    Unit::process();
    if (buffer != nullptr) {
        buffer->lock();
        if (buffer->getNumChannels() > 0 && buffer->getBufferSize() > 0) {
            unsigned int start = index % buffer->getBufferSize();
            for (unsigned int i = 0; i < getNumChannels(); i++) {
                std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
                unsigned int position = start;
                for (unsigned int k = 0; k < getBufferSize(); k++) {
                    buffer->getChannel(i % buffer->getNumChannels())[position] = inputBuffer[k];
                    position++;
                    if (position >= buffer->getBufferSize()) {
                        position -= buffer->getBufferSize();
                    }
                }
            }
        }
        buffer->unlock();
    }
}
