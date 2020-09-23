#include "Comparison.h"

dsp::Comparison::Comparison(Type type, Space space)
        : Transformer(type, Type::BINARY, space)
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
    switch (mode) {
        case Mode::EQUAL_TO: transform(getThreshold(), [](Sample x, Sample y) { return x == y; }); break;
        case Mode::GREATER_THAN: transform(getThreshold(), [](Sample x, Sample y) { return x > y; }); break;
        case Mode::LESS_THAN: transform(getThreshold(), [](Sample x, Sample y) { return x < y; }); break;
    }
}
