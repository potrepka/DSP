#include "SampleDuration.h"

dsp::SampleDuration::SampleDuration(Space space)
        : Generator(Type::SECONDS, space) {}

void dsp::SampleDuration::setSampleRateNoLock(unsigned int sampleRate) {
    Unit::setSampleRateNoLock(sampleRate);
    getOutputSignal()->setDefaultValue(getOneOverSampleRate());
}
