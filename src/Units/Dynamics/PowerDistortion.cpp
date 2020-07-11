#include "PowerDistortion.h"

const unsigned int dsp::PowerDistortion::DRIVE = 1;

dsp::PowerDistortion::PowerDistortion(Space space) : Processor(Type::BIPOLAR, Type::BIPOLAR, space) {
    pushInput(Type::RATIO, space);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::PowerDistortion::getDrive() const {
    return getInput(DRIVE);
}

void dsp::PowerDistortion::process() {
    Unit::process();
    transform(getDrive(), [](DSP_FLOAT x, DSP_FLOAT y) {
        if (y == 0.0) {
            return 0.0;
        } else {
            x = clip(x, -1.0, 1.0);
            return x < 0.0 ? pow(1.0 + x, y) - 1.0 : 1.0 - pow(1.0 - x, y);
        }
    });
}
