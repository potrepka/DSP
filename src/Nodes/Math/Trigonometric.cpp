#include "Trigonometric.h"

dsp::Trigonometric::Trigonometric(Domain domain)
    : Transformer(Type::RATIO, Type::RATIO, domain), mode(Mode::SINE) {}

dsp::Trigonometric::Mode dsp::Trigonometric::getMode() const { return mode; }

void dsp::Trigonometric::setMode(Mode mode) {
  lock();
  this->mode = mode;
  unlock();
}

void dsp::Trigonometric::processNoLock() {
  switch (mode) {
    case Mode::SINE:
      getOutput()->getWrapper().replaceWithApplicationOf(
          [](Sample x) { return sin(x); }, getInput()->getWrapper());
      break;
    case Mode::COSINE:
      getOutput()->getWrapper().replaceWithApplicationOf(
          [](Sample x) { return cos(x); }, getInput()->getWrapper());
      break;
    case Mode::TANGENT:
      getOutput()->getWrapper().replaceWithApplicationOf(
          [](Sample x) { return tan(x); }, getInput()->getWrapper());
      break;
  }
}
