#include "BufferOscillator.h"

const unsigned int dsp::BufferOscillator::PHASE = 0;

dsp::BufferOscillator::BufferOscillator() : Generator(Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::UNIPOLAR);
}

std::vector<DSP_FLOAT> &dsp::BufferOscillator::getBuffer(unsigned int channel) {
    return *buffers[channel];
}

void dsp::BufferOscillator::setBuffer(unsigned int channel, std::vector<DSP_FLOAT> &buffer) {
    lock();
    buffers[channel] = &buffer;
    unlock();
}

void dsp::BufferOscillator::removeBuffer(unsigned int channel) {
    lock();
    buffers[channel] = nullptr;
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::BufferOscillator::getPhase() {
    return getInput(PHASE);
}

void dsp::BufferOscillator::setNumChannelsNoLock(unsigned int numChannels) {
    Unit::setNumChannelsNoLock(numChannels);
    buffers.resize(numChannels, nullptr);
}

void dsp::BufferOscillator::process() {
    Unit::process();
    for (unsigned int i = 0; i < getNumChannels(); i++) {
        if (buffers[i] != nullptr && buffers[i]->size() > 0) {
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                outputBuffer[k] = linear(*buffers[i], wrap(phaseBuffer[k], 0.0, 1.0) * buffers[i]->size());
            }
        }
    }
}
