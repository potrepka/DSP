#include "BufferDuration.h"

dsp::BufferDuration::BufferDuration(Space space)
        : Generator(Type::SECONDS, space) {}

void dsp::BufferDuration::setSampleRateNoLock(unsigned int sampleRate) {
    Unit::setSampleRateNoLock(sampleRate);
    getOutputSignal()->setDefaultValue(getOneOverSampleRate() * getBufferSize());
}

void dsp::BufferDuration::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    getOutputSignal()->setDefaultValue(getOneOverSampleRate() * getBufferSize());
}
