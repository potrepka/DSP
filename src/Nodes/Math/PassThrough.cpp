#include "PassThrough.h"

dsp::PassThrough::PassThrough(Type type, Space space)
        : Transformer(type, type, space) {}

dsp::PassThrough::PassThrough(Type inType, Type outType, Space space)
        : Transformer(inType, outType, space) {}

dsp::PassThrough::PassThrough(Type inType, Type outType, Space inSpace, Space outSpace)
        : Transformer(inType, outType, inSpace, outSpace) {}

void dsp::PassThrough::processNoLock() {
    getOutput()->getWrapper().copyFrom(getInput()->getWrapper());
}
