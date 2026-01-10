#include "Identity.h"

dsp::Identity::Identity(Type type, Space space) : Transformer(type, space) {}

dsp::Identity::Identity(Type inputType, Type outputType, Space space)
    : Transformer(inputType, outputType, space) {}

dsp::Identity::Identity(Type inputType, Type outputType, Space inSpace,
                        Space outSpace)
    : Transformer(inputType, outputType, inSpace, outSpace) {}

void dsp::Identity::processNoLock() {
  getOutput()->getWrapper().copyFrom(getInput()->getWrapper());
}
