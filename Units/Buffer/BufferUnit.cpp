#include "BufferUnit.h"

dsp::BufferUnit::BufferUnit() : Consumer(Connection::Type::BIPOLAR), unitBufferSize(0), index(0) {}

void dsp::BufferUnit::setNumChannels(std::size_t size) {
    lock();
    for (const auto& input : inputs) {
        input->setNumChannels(size);
    }
    for (const auto& output : outputs) {
        output->setNumChannels(size);
    }
    std::size_t numChannels = getNumChannels();
    buffers.resize(numChannels, std::vector<DSP_FLOAT>(unitBufferSize, 0));
    unlock();
}

std::size_t dsp::BufferUnit::getUnitBufferSize() {
    return unitBufferSize;
}

void dsp::BufferUnit::setUnitBufferSize(std::size_t size) {
    lock();
    for (int i = 0; i < buffers.size(); i++) {
        buffers[i].resize(size);
    }
    unitBufferSize = size;
    unlock();
}

std::vector<DSP_FLOAT>& dsp::BufferUnit::getUnitBuffer(std::size_t index) {
    return buffers[index];
}

void dsp::BufferUnit::process() {
    Consumer::process();
    size_t start = index % unitBufferSize;
    for (int i = 0; i < getNumChannels(); i++) {
        std::vector<DSP_FLOAT>& inputBuffer = getInputSignal()->getChannel(i)->getBuffer();
        std::size_t position = start;
        for (int k = 0; k < getBufferSize(); k++) {
            buffers[i][position] = inputBuffer[k];
            position++;
            if (position >= unitBufferSize) {
                position -= unitBufferSize;
            }
        }
    }
}
