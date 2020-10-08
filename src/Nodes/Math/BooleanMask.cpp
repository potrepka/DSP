#include "BooleanMask.h"

dsp::BooleanMask::BooleanMask(Type type, Space space)
        : Transformer(type, type, space)
        , mask(std::make_shared<Input>(Type::BOOLEAN, space, 1.0)) {
    getInputs().push_back(mask);
}

std::shared_ptr<dsp::Input> dsp::BooleanMask::getMask() const {
    return mask;
}

void dsp::BooleanMask::processNoLock() {
    transform(getMask(), [](Sample x, Sample y) { return y ? x : 0.0; });
}
