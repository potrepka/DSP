#include "BufferUnit.h"

dsp::BufferUnit::BufferUnit() : Consumer(Connection::Type::BIPOLAR), unitBufferSize(0), index(0) {}

unsigned int dsp::BufferUnit::getUnitBufferSize() {
    return unitBufferSize;
}

void dsp::BufferUnit::setUnitBufferSize(unsigned int size) {
    lock();
    for (unsigned int i = 0; i < buffers.size(); i++) {
        buffers[i].resize(size);
    }
    unitBufferSize = size;
    unlock();
}

std::vector<DSP_FLOAT> &dsp::BufferUnit::getUnitBuffer(unsigned int channel) {
    return buffers[channel];
}

void dsp::BufferUnit::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    buffers.resize(numChannels, std::vector<DSP_FLOAT>(unitBufferSize, 0));
}

void dsp::BufferUnit::process() {
    Unit::process();
    if (unitBufferSize > 0) {
        unsigned int start = index % unitBufferSize;
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
            unsigned int position = start;
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                buffers[i][position] = inputBuffer[k];
                position++;
                if (position >= unitBufferSize) {
                    position -= unitBufferSize;
                }
            }
        }
    }
}
