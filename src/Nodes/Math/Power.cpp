#include "Power.h"

dsp::Power::Power(Type type, Space space)
        : Transformer(type, type, space)
        , exponent(std::make_shared<Input>(Type::RATIO, space)) {
    getInputs().push_back(exponent);
}

std::shared_ptr<dsp::Input> dsp::Power::getExponent() const {
    return exponent;
}

void dsp::Power::processNoLock() {
    getOutput()->getWrapper().replaceWithProductOf(getInput()->getWrapper(), getExponent()->getWrapper());
}
