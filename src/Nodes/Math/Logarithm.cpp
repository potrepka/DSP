#include "Logarithm.h"

dsp::Logarithm::Logarithm(Space space)
        : Transformer(Type::RATIO, Type::RATIO, space)
        , base(std::make_shared<Input>(Type::RATIO, space)) {
    getInputs().push_back(base);
}

std::shared_ptr<dsp::Input> dsp::Logarithm::getBase() const {
    return base;
}

void dsp::Logarithm::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf(
            [](Sample x, Sample y) { return log2(x) / log2(y); }, getInput()->getWrapper(), getBase()->getWrapper());
}
