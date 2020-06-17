#include "SampleRate.h"

dsp::SampleRate::SampleRate() : Generator(Connection::Type::INTEGER) {}

void dsp::SampleRate::setSampleRateNoLock(unsigned int sampleRate) {
    Unit::setSampleRateNoLock(sampleRate);
    getOutputSignal()->setValue(sampleRate);
}
