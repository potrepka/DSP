#include "Division.h"

dsp::Division::Division(Type type, Space space)
        : Transformer(type, type, space)
        , divisor(std::make_shared<Input>(Type::RATIO, space)) {
    getInputs().push_back(divisor);
}

std::shared_ptr<dsp::Input> dsp::Division::getDivisor() const {
    return divisor;
}

void dsp::Division::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf(
            [](Sample x, Sample y) { return x == 0.0 && y == 0.0 ? 0.0 : x / y; },
            getInput()->getWrapper(),
            getDivisor()->getWrapper());
}
