#include "Negative.h"

dsp::Negative::Negative(Type type, Domain domain) : Transformer(type, domain) {}

void dsp::Negative::processNoLock() {
  getOutput()->getWrapper().replaceWithNegativeOf(getInput()->getWrapper());
}
