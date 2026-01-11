#include "AbsoluteValue.h"

dsp::AbsoluteValue::AbsoluteValue(Type type, Domain domain)
    : Transformer(type, domain) {}

void dsp::AbsoluteValue::processNoLock() {
  getOutput()->getWrapper().replaceWithAbsoluteValueOf(
      getInput()->getWrapper());
}
