#include "Logarithm.h"

dsp::Logarithm::Logarithm(Domain domain)
    : Transformer(Type::RATIO, Type::RATIO, domain),
      base(std::make_shared<Input>(Type::RATIO, domain)) {
  getInputs().push_back(base);
}

std::shared_ptr<dsp::Input> dsp::Logarithm::getBase() const { return base; }

void dsp::Logarithm::processNoLock() {
  getOutput()->getWrapper().replaceWithApplicationOf(
      [](Sample x, Sample y) { return log2(x) / log2(y); },
      getInput()->getWrapper(), getBase()->getWrapper());
}
