#include "BooleanMask.h"

dsp::BooleanMask::BooleanMask(Type type, Space space)
        : Transformer(type, type, space)
        , mask(std::make_shared<Input>(Type::BOOLEAN)) {
    getInputs().push_back(mask);
}

std::shared_ptr<dsp::Input> dsp::BooleanMask::getMask() const {
    return mask;
}

void dsp::BooleanMask::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf(
            [](Sample x, Sample y) { return y ? x : 0.0; }, getInput()->getWrapper(), getMask()->getWrapper());
}
