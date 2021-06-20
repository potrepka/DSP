#include "Power.h"

dsp::Power::Power(Space space)
        : Transformer(Type::RATIO, Type::RATIO, space)
        , exponent(std::make_shared<Input>(Type::RATIO, space)) {
    getInputs().push_back(exponent);
}

std::shared_ptr<dsp::Input> dsp::Power::getExponent() const {
    return exponent;
}

void dsp::Power::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf(
            [](Sample x, Sample y) { return pow(x, y); }, getInput()->getWrapper(), getExponent()->getWrapper());
}
