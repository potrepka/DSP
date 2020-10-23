#include "Comparison.h"

dsp::Comparison::Comparison(Type type, Space space)
        : Transformer(type, Type::BOOLEAN, space)
        , mode(Mode::EQUAL_TO)
        , threshold(std::make_shared<Input>(type, space)) {
    getInputs().push_back(threshold);
}

dsp::Comparison::Mode dsp::Comparison::getMode() const {
    return mode;
}

void dsp::Comparison::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::shared_ptr<dsp::Input> dsp::Comparison::getThreshold() const {
    return threshold;
}

void dsp::Comparison::processNoLock() {
    getOutput()->getWrapper().replaceWithApplicationOf(
            [this](Sample x, Sample y) {
                switch (mode) {
                    case Mode::EQUAL_TO: return x == y;
                    case Mode::GREATER_THAN: return x > y;
                    case Mode::LESS_THAN: return x < y;
                }
            },
            getInput()->getWrapper(),
            getThreshold()->getWrapper());
}
