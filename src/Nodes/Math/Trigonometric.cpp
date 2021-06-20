#include "Trigonometric.h"

dsp::Trigonometric::Trigonometric(Space space)
        : Transformer(Type::RATIO, Type::RATIO, space)
        , mode(std::make_shared<Input>(Type::INTEGER, space, Mode::MAX)) {
    getInputs().push_back(mode);
}

std::shared_ptr<dsp::Input> dsp::Trigonometric::getMode() const {
    return mode;
}

void dsp::Trigonometric::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf(
            [](Sample x, Sample y) {
                switch (static_cast<int>(y)) {
                    case Mode::SINE: return sin(x);
                    case Mode::COSINE: return cos(x);
                    case Mode::TANGENT: return tan(x);
                }
            },
            getInput()->getWrapper(),
            getMode()->getWrapper());
}
