#include "Negative.h"

dsp::Negative::Negative(Type type, Space space)
    : Transformer(type, space) {}

void dsp::Negative::processNoLock() {
  getOutput()->getWrapper().replaceWithNegativeOf(getInput()->getWrapper());
}
