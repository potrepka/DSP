#include "Distortion.h"

dsp::Distortion::Distortion(Space space)
        : Transformer(Type::RATIO, Type::RATIO, space)
        , drive(std::make_shared<Input>(Type::RATIO, space, 1.0)) {
    getInputs().push_back(drive);
}

std::shared_ptr<dsp::Input> dsp::Distortion::getDrive() const {
    return drive;
}

void dsp::Distortion::processNoLock() {
    transform(getDrive(), [](Sample x, Sample y) {
        if (x == 0.0 || y == 0.0) {
            return 0.0;
        }
        if (y == 1.0) {
            return x;
        }
        if (std::isinf(y)) {
            return x < 0.0 ? -1.0 : 1.0;
        }
        Sample yy = y * y;
        Sample minus = yy - 1;
        if (x < 0.0) {
            return (yy / (1.0 - minus * x) - yy) / minus;
        } else {
            return (yy - yy / (1.0 + minus * x)) / minus;
        }
    });
}
