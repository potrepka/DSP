#include "Function.h"

dsp::Function::Function(Type type, Domain domain)
    : Transformer(type, domain),
      a(getInput()),
      b(std::make_shared<Input>(type, domain)) {
  getInputs().push_back(b);
}

dsp::Function::Function(Type aType, Type bType, Type outputType, Domain domain)
    : Transformer(aType, outputType, domain),
      a(getInput()),
      b(std::make_shared<Input>(bType, domain)) {
  getInputs().push_back(b);
}

std::shared_ptr<std::function<dsp::Sample(dsp::Sample, dsp::Sample)>>
dsp::Function::getFunction() const {
  return function;
}

void dsp::Function::setFunction(
    std::shared_ptr<std::function<Sample(Sample, Sample)>> function) {
  this->function = function;
}

std::shared_ptr<dsp::Input> dsp::Function::getA() const { return a; }

std::shared_ptr<dsp::Input> dsp::Function::getB() const { return b; }

void dsp::Function::processNoLock() {
  if (function != nullptr) {
    getOutput()->getWrapper().replaceWithApplicationOf(
        *function, getA()->getWrapper(), getB()->getWrapper());
  }
}
