#include "BufferSize.h"

dsp::BufferSize::BufferSize() : Generator(Connection::Type::INTEGER) {}

void dsp::BufferSize::setBufferSize(unsigned int bufferSize) {
    lock();
    Unit::setBufferSizeNoLock(bufferSize);
    getOutputSignal()->setValue(bufferSize);
    unlock();
}
