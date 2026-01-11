#include "NotGate.h"

dsp::NotGate::NotGate(Domain domain)
    : Transformer(Type::BOOLEAN, Type::BOOLEAN, domain) {}

void dsp::NotGate::processNoLock() {
  getOutput()->getWrapper().replaceWithApplicationOf(
      [](Sample x) { return !x; }, getInput()->getWrapper());
}
