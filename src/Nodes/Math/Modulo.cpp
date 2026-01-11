#include "Modulo.h"

dsp::Modulo::Modulo(Type type, Domain domain)
    : Transformer(type, domain),
      divisor(std::make_shared<Input>(type, domain, 0.0, 1.0)) {
  getInputs().push_back(divisor);
}

std::shared_ptr<dsp::Input> dsp::Modulo::getDivisor() const { return divisor; }

void dsp::Modulo::processNoLock() {
  getOutput()->getWrapper().replaceWithApplicationOf(
      wrap, getInput()->getWrapper(), getDivisor()->getWrapper());
}
