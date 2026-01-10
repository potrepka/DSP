#include "Comparison.h"

dsp::Comparison::Comparison(Type type, Space space)
    : Transformer(type, Type::BOOLEAN, space),
      threshold(std::make_shared<Input>(type, space)) {
  getInputs().push_back(threshold);
}

dsp::Comparison::Mode dsp::Comparison::getMode() const { return mode; }

void dsp::Comparison::setMode(Mode mode) {
  lock();
  this->mode = mode;
  unlock();
}

std::shared_ptr<dsp::Input> dsp::Comparison::getThreshold() const {
  return threshold;
}

void dsp::Comparison::processNoLock() {
  switch (mode) {
    case Mode::EQUAL:
      getOutput()->getWrapper().replaceWithApplicationOf(
          [](Sample x, Sample y) { return x == y ? 1.0 : 0.0; },
          getInput()->getWrapper(), getThreshold()->getWrapper());
      break;
    case Mode::NOT_EQUAL:
      getOutput()->getWrapper().replaceWithApplicationOf(
          [](Sample x, Sample y) { return x != y ? 1.0 : 0.0; },
          getInput()->getWrapper(), getThreshold()->getWrapper());
      break;
    case Mode::LESS_THAN:
      getOutput()->getWrapper().replaceWithApplicationOf(
          [](Sample x, Sample y) { return x < y ? 1.0 : 0.0; },
          getInput()->getWrapper(), getThreshold()->getWrapper());
      break;
    case Mode::GREATER_THAN:
      getOutput()->getWrapper().replaceWithApplicationOf(
          [](Sample x, Sample y) { return x > y ? 1.0 : 0.0; },
          getInput()->getWrapper(), getThreshold()->getWrapper());
      break;
    case Mode::LESS_THAN_OR_EQUAL:
      getOutput()->getWrapper().replaceWithApplicationOf(
          [](Sample x, Sample y) { return x <= y ? 1.0 : 0.0; },
          getInput()->getWrapper(), getThreshold()->getWrapper());
      break;
    case Mode::GREATER_THAN_OR_EQUAL:
      getOutput()->getWrapper().replaceWithApplicationOf(
          [](Sample x, Sample y) { return x >= y ? 1.0 : 0.0; },
          getInput()->getWrapper(), getThreshold()->getWrapper());
      break;
  }
}
