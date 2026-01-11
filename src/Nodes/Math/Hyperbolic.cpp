#include "Hyperbolic.h"

dsp::Hyperbolic::Hyperbolic(Domain domain)
    : Transformer(Type::RATIO, Type::RATIO, domain), mode(Mode::SINE) {}

dsp::Hyperbolic::Mode dsp::Hyperbolic::getMode() const { return mode; }

void dsp::Hyperbolic::setMode(Mode mode) {
  lock();
  this->mode = mode;
  unlock();
}

void dsp::Hyperbolic::processNoLock() {
  switch (mode) {
    case Mode::SINE:
      getOutput()->getWrapper().replaceWithApplicationOf(
          [](Sample x) { return sinh(x); }, getInput()->getWrapper());
      break;
    case Mode::COSINE:
      getOutput()->getWrapper().replaceWithApplicationOf(
          [](Sample x) { return cosh(x); }, getInput()->getWrapper());
      break;
    case Mode::TANGENT:
      getOutput()->getWrapper().replaceWithApplicationOf(
          [](Sample x) { return tanh(x); }, getInput()->getWrapper());
      break;
  }
}
