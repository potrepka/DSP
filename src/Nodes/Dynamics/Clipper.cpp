#include "Clipper.h"

dsp::Clipper::Clipper(Space space)
        : Transformer(Type::RATIO, Type::RATIO, space) {}

dsp::Clipper::Mode dsp::Clipper::getMode() const {
    return mode;
}

void dsp::Clipper::setMode(Mode mode) {
    this->mode = mode;
}

void dsp::Clipper::processNoLock() {
    transform([this](Sample x) {
        switch (mode) {
            case Mode::CLIP: return clip(x, -1.0, 1.0);
            case Mode::WRAP: return wrap(1.0 + x, 2.0) - 1.0;
            case Mode::MIRROR: {
                Sample onePlusX = 1.0 + x;
                return (wrap(onePlusX, 2.0) - 1.0) * (wrap(onePlusX, 4.0) < 2.0 ? 1.0 : -1.0);
            }
        }
    });
}
