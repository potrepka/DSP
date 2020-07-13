#include "BufferSize.h"

dsp::BufferSize::BufferSize(Space space) : Generator(Type::INTEGER, space) {}

void dsp::BufferSize::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    getOutputSignal()->setDefaultValue(bufferSize);
}
