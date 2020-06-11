#include "SampleRate.h"

dsp::SampleRate::SampleRate() : Generator(Connection::Type::INTEGER) {}

void dsp::SampleRate::setSampleRate(unsigned int sampleRate) {
    lock();
    setSampleRateNoLock(sampleRate);
    getOutputSignal()->setValue(sampleRate);
    unlock();
}
