#include "BufferRate.h"

dsp::BufferRate::BufferRate(Space space)
        : Generator(Type::HERTZ, space) {}

void dsp::BufferRate::setSampleRateNoLock(unsigned int sampleRate) {
    Unit::setSampleRateNoLock(sampleRate);
    getOutputSignal()->setDefaultValue(getSampleRate() * getOneOverBufferSize());
}

void dsp::BufferRate::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    getOutputSignal()->setDefaultValue(getSampleRate() * getOneOverBufferSize());
}
