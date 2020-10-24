#include "Comparison.h"

dsp::Comparison::Comparison(Type type, Space space)
        : Transformer(type, Type::BOOLEAN, space)
        , threshold(std::make_shared<Input>(type, space))
        , mode(std::make_shared<Input>(Type::INTEGER, space)) {
    getInputs().push_back(threshold);
    getInputs().push_back(mode);
}

std::shared_ptr<dsp::Input> dsp::Comparison::getThreshold() const {
    return threshold;
}

std::shared_ptr<dsp::Input> dsp::Comparison::getMode() const {
    return mode;
}

void dsp::Comparison::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf(
            [this](Sample x, Sample y, Sample z) {
                const Sample modeClipped = clip(z, Mode::MIN, Mode::MAX);
                switch (static_cast<int>(modeClipped)) {
                    case Mode::EQUAL_TO: return x == y;
                    case Mode::GREATER_THAN: return x > y;
                    case Mode::LESS_THAN: return x < y;
                }
            },
            getInput()->getWrapper(),
            getThreshold()->getWrapper(),
            getMode()->getWrapper());
}
