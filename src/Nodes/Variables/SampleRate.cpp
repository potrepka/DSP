#include "SampleRate.h"

dsp::SampleRate::SampleRate()
        : Producer(Type::HERTZ) {}

void dsp::SampleRate::setSampleRateNoLock(double sampleRate) {
    Node::setSampleRateNoLock(sampleRate);
    getOutput()->setAllChannelValues(getSampleRate());
}
