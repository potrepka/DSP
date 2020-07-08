#include "BufferSize.h"

dsp::BufferSize::BufferSize() : Generator(Type::RATIO) {}

void dsp::BufferSize::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    getOutputSignal()->setDefaultValue(bufferSize);
}
