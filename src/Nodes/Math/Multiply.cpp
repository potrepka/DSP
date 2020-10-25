#include "Multiply.h"

dsp::Multiply::Multiply(Type type, Space space)
        : Transformer(type, type, space)
        , factor(std::make_shared<Input>(Type::RATIO, space)) {
    getInputs().push_back(factor);
}

std::shared_ptr<dsp::Input> dsp::Multiply::getFactor() const {
    return factor;
}

void dsp::Multiply::processNoLock() {
    getOutput()->getWrapper().replaceWithProductOf(getInput()->getWrapper(), getFactor()->getWrapper());
}
