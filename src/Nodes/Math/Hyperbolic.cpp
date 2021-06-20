#include "Hyperbolic.h"

dsp::Hyperbolic::Hyperbolic(Space space)
        : Transformer(Type::RATIO, Type::RATIO, space)
        , mode(std::make_shared<Input>(Type::INTEGER, space, Mode::MAX)) {
    getInputs().push_back(mode);
}

std::shared_ptr<dsp::Input> dsp::Hyperbolic::getMode() const {
    return mode;
}

void dsp::Hyperbolic::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf(
            [](Sample x, Sample y) {
                switch (static_cast<int>(y)) {
                    case Mode::SINE: return sinh(x);
                    case Mode::COSINE: return cosh(x);
                    case Mode::TANGENT: return tanh(x);
                }
            },
            getInput()->getWrapper(),
            getMode()->getWrapper());
}
