#include "Multiply.h"

dsp::Multiply::Multiply(Type type, Space space)
        : Transformer(type, type == Type::INTEGER ? Type::LOGARITHMIC : type, space)
        , factor(std::make_shared<Input>(type, space, 1.0)) {
    getInputs().push_back(factor);
}

std::shared_ptr<dsp::Input> dsp::Multiply::getFactor() const {
    return factor;
}

void dsp::Multiply::processNoLock() {
    getOutput()->getBlock().replaceWithProductOf(getInput()->getBlock(), getFactor()->getBlock());
}
