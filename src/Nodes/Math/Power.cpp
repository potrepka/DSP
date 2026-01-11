#include "Power.h"

dsp::Power::Power(Domain domain)
    : Transformer(Type::RATIO, Type::RATIO, domain),
      exponent(std::make_shared<Input>(Type::RATIO, domain)) {
  getInputs().push_back(exponent);
}

std::shared_ptr<dsp::Input> dsp::Power::getExponent() const { return exponent; }

void dsp::Power::processNoLock() {
  getOutput()->getWrapper().replaceWithApplicationOf(
      [](Sample x, Sample y) { return pow(x, y); }, getInput()->getWrapper(),
      getExponent()->getWrapper());
}
