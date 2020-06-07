#include "BufferSize.h"

dsp::BufferSize::BufferSize() : Generator(Connection::Type::INTEGER) {}

void dsp::BufferSize::setBufferSize(unsigned int bufferSize) {
    Generator::setBufferSize(bufferSize);
    getOutputSignal()->setValue(bufferSize);
}
