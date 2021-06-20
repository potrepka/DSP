#include "BufferDuration.h"

dsp::BufferDuration::BufferDuration()
        : Producer(Type::SECONDS) {}

void dsp::BufferDuration::setNumSamplesNoLock(size_t numSamples) {
    Node::setNumSamplesNoLock(numSamples);
    getOutput()->setAllChannelValues(getOneOverSampleRate() * getNumSamples());
}

void dsp::BufferDuration::setSampleRateNoLock(double sampleRate) {
    Node::setSampleRateNoLock(sampleRate);
    getOutput()->setAllChannelValues(getOneOverSampleRate() * getNumSamples());
}
