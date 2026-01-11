#include "Reciprocal.h"

dsp::Reciprocal::Reciprocal(Type type, Domain domain)
    : Transformer(type,
                  type == Type::HERTZ     ? Type::SECONDS
                  : type == Type::SECONDS ? Type::HERTZ
                                          : type,
                  domain) {}

void dsp::Reciprocal::processNoLock() {
  getOutput()->getWrapper().replaceWithApplicationOf(
      [](Sample x) { return 1.0 / x; }, getInput()->getWrapper());
}
