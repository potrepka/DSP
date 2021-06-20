#include "Modulo.h"

dsp::Modulo::Modulo(Type type, Space space)
        : Transformer(type, type, space)
        , divisor(std::make_shared<Input>(type, space, 0.0, 1.0)) {
    getInputs().push_back(divisor);
}

std::shared_ptr<dsp::Input> dsp::Modulo::getDivisor() const {
    return divisor;
}

void dsp::Modulo::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf(wrap, getInput()->getWrapper(), getDivisor()->getWrapper());
}
