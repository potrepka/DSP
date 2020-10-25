#include "Divide.h"

dsp::Divide::Divide(Type type, Space space)
        : Transformer(type, type, space)
        , divisor(std::make_shared<Input>(Type::RATIO, space)) {
    getInputs().push_back(divisor);
}

std::shared_ptr<dsp::Input> dsp::Divide::getDivisor() const {
    return divisor;
}

void dsp::Divide::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf(
            [](Sample x, Sample y) { return x == 0.0 && y == 0.0 ? 0.0 : x / y; },
            getInput()->getWrapper(),
            getDivisor()->getWrapper());
}
