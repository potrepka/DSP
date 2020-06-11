#include "BufferOscillator.h"

const std::size_t dsp::BufferOscillator::PHASE = 0;

dsp::BufferOscillator::BufferOscillator() : Generator(Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::UNIPOLAR);
}

void dsp::BufferOscillator::setNumChannels(std::size_t numChannels) {
    lock();
    setNumChannelsNoLock(numChannels);
    buffers.resize(numChannels, nullptr);
    unlock();
}

std::vector<DSP_FLOAT> &dsp::BufferOscillator::getBuffer(std::size_t index) {
    return *buffers[index];
}

void dsp::BufferOscillator::setBuffer(std::size_t index, std::vector<DSP_FLOAT> &buffer) {
    lock();
    buffers[index] = &buffer;
    unlock();
}

void dsp::BufferOscillator::removeBuffer(std::size_t index) {
    lock();
    buffers[index] = nullptr;
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::BufferOscillator::getPhase() {
    return getInput(PHASE);
}

void dsp::BufferOscillator::process() {
    Unit::process();
    for (std::size_t i = 0; i < getNumChannels(); i++) {
        if (buffers[i] != nullptr && buffers[i]->size() > 0) {
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
            for (int k = 0; k < getBufferSize(); k++) {
                outputBuffer[k] = linear(*buffers[i], phaseBuffer[k] * buffers[i]->size());
            }
        }
    }
}
