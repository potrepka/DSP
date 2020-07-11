#include "BufferUnit.h"

dsp::BufferUnit::BufferUnit(unsigned int bufferSize) : Consumer(Type::BIPOLAR), writeIndex(0) {
    buffer = std::make_shared<Buffer>(0, bufferSize, Type::BIPOLAR);
}

std::shared_ptr<dsp::Buffer> dsp::BufferUnit::getBuffer() const {
    return buffer;
}

void dsp::BufferUnit::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    buffer->setNumChannels(numChannels);
}

void dsp::BufferUnit::process() {
    Unit::process();
    buffer->lock();
    if (buffer->getBufferSize() > 0) {
        writeIndex %= buffer->getBufferSize();
        unsigned int start = writeIndex;
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
            unsigned int index = start;
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                buffer->getChannel(i)[index] = inputBuffer[k];
                index++;
                if (index >= buffer->getBufferSize()) {
                    index -= buffer->getBufferSize();
                }
            }
        }
        writeIndex += getBufferSize();
    }
    buffer->unlock();
}
