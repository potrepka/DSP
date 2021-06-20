#include "SampleDuration.h"

dsp::SampleDuration::SampleDuration()
        : Producer(Type::SECONDS) {}

void dsp::SampleDuration::setSampleRateNoLock(double sampleRate) {
    Node::setSampleRateNoLock(sampleRate);
    getOutput()->setAllChannelValues(getOneOverSampleRate());
}
