#include "SampleRate.h"

dsp::SampleRate::SampleRate() : Generator(Type::HERTZ) {}

void dsp::SampleRate::setSampleRateNoLock(unsigned int sampleRate) {
    Unit::setSampleRateNoLock(sampleRate);
    getOutputSignal()->setDefaultValue(sampleRate);
}
