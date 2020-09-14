#include "PassThrough.h"

dsp::PassThrough::PassThrough(Type type, Space space)
        : Transformer(type, type, space) {}

void dsp::PassThrough::processNoLock() {
    getOutput()->getWrapper().copyFrom(getInput()->getWrapper());
}