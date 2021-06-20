#include "Multiplication.h"

dsp::Multiplication::Multiplication(Type type, Space space)
        : Transformer(type, type, space)
        , factor(std::make_shared<Input>(Type::RATIO)) {
    getInputs().push_back(factor);
}

std::shared_ptr<dsp::Input> dsp::Multiplication::getFactor() const {
    return factor;
}

void dsp::Multiplication::processNoLock() {
    getOutput()->getWrapper().replaceWithProductOf(getInput()->getWrapper(), getFactor()->getWrapper());
}
