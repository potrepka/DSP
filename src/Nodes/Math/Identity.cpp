#include "Identity.h"

dsp::Identity::Identity(Type type, Space space)
        : Transformer(type, type, space) {}

dsp::Identity::Identity(Type inType, Type outType, Space space)
        : Transformer(inType, outType, space) {}

dsp::Identity::Identity(Type inType, Type outType, Space inSpace, Space outSpace)
        : Transformer(inType, outType, inSpace, outSpace) {}

void dsp::Identity::processNoLock() {
    getOutput()->getWrapper().copyFrom(getInput()->getWrapper());
}
