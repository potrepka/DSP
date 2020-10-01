#include "Shaper.h"

dsp::Shaper::Shaper(Space space)
        : Transformer(Type::RATIO, Type::RATIO, space)
        , mode(Mode::POLYNOMIAL)
        , drive(std::make_shared<Input>(Type::RATIO, space, 1.0)) {
    getInputs().push_back(drive);
}

dsp::Shaper::Mode dsp::Shaper::getMode() const {
    return mode;
}

void dsp::Shaper::setMode(Mode mode) {
    this->mode = mode;
}

std::shared_ptr<dsp::Input> dsp::Shaper::getDrive() const {
    return drive;
}

void dsp::Shaper::processNoLock() {
    transform(getDrive(), [this](Sample x, Sample y) {
        if (y == 0.0) {
            return 0.0;
        }
        if (y == 1.0) {
            return x;
        }
        switch (mode) {
            case Mode::POLYNOMIAL: {
                Sample onePlusX = 1.0 + x;
                Sample oneMinusX = 1.0 - x;
                Sample yMinusOne = y - 1.0;
                return x < 0.0 ? onePlusX * pow(abs(onePlusX), yMinusOne) - 1.0
                               : 1.0 - oneMinusX * pow(oneMinusX, yMinusOne);
            }
            case Mode::HYPERBOLIC: {
                Sample yMinusOne = y - 1.0;
                return x < 0.0 ? (y / (1.0 - yMinusOne * x) - y) / yMinusOne
                               : (y - y / (1.0 + yMinusOne * x)) / yMinusOne;
            }
        }
    });
}
