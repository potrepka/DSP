#include "BufferSize.h"

dsp::BufferSize::BufferSize() : Generator(Connection::Type::LINEAR) {}

void dsp::BufferSize::setBufferSizeNoLock(unsigned int bufferSize) {
    Unit::setBufferSizeNoLock(bufferSize);
    getOutputSignal()->setValue(bufferSize);
}
