#include "SampleRate.h"

dsp::SampleRate::SampleRate() : Generator(Connection::Type::INTEGER) {}

void dsp::SampleRate::setSampleRate(unsigned int sampleRate) {
    lock();
    Unit::setSampleRateNoLock(sampleRate);
    getOutputSignal()->setValue(sampleRate);
    unlock();
}
