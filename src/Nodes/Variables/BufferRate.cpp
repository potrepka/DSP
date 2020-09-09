#include "BufferRate.h"

dsp::BufferRate::BufferRate()
        : Producer(Type::HERTZ) {}

void dsp::BufferRate::setNumSamplesNoLock(int numSamples) {
    Node::setNumSamplesNoLock(numSamples);
    getOutput()->setAllChannelValues(getSampleRate() * getOneOverNumSamples());
}

void dsp::BufferRate::setSampleRateNoLock(double sampleRate) {
    Node::setSampleRateNoLock(sampleRate);
    getOutput()->setAllChannelValues(getSampleRate() * getOneOverNumSamples());
}
