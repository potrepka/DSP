#include "SampleRate.h"

dsp::SampleRate::SampleRate(Space space)
        : Generator(Type::HERTZ, space) {}

void dsp::SampleRate::setSampleRateNoLock(unsigned int sampleRate) {
    Unit::setSampleRateNoLock(sampleRate);
    getOutputSignal()->setDefaultValue(sampleRate);
}
