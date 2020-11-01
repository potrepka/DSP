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
                    case Mode::SINE: return 0.5 * (exp(x) - exp(-x));
                    case Mode::COSINE: return 0.5 * (exp(x) + exp(-x));
                    case Mode::TANGENT: {
                        const Sample expSquared = exp(2.0 * x);
                        return (expSquared - 1) / (expSquared + 1);
                    }
                }
            },
            getInput()->getWrapper(),
            getMode()->getWrapper());
}
