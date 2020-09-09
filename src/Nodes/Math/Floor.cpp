#include "Floor.h"

dsp::Floor::Floor(Type type, Space space)
        : Transformer(type, type == Type::INTEGER ? Type::LOGARITHMIC : type, space)
        , divisor(std::make_shared<Input>(type, space, 1.0)) {
    getInputs().push_back(divisor);
}

std::shared_ptr<dsp::Input> dsp::Floor::getDivisor() const {
    return divisor;
}

void dsp::Floor::processNoLock() {
    transform(getDivisor(), [](Sample x, Sample y) { return y == 0.0 ? x : floor(x / y) * y; });
}
