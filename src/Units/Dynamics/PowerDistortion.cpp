#include "PowerDistortion.h"

dsp::PowerDistortion::PowerDistortion(Space space)
        : Processor(Type::BIPOLAR, Type::BIPOLAR, space)
        , drive(pushInput(Type::RATIO, space, 1.0)) {}

std::shared_ptr<dsp::InputParameter> dsp::PowerDistortion::getDrive() const {
    return drive;
}

void dsp::PowerDistortion::process() {
    Unit::process();
#if DSP_USE_VC
    transform(getDrive(), [](Vector x, Vector y) {
        return Vc::iif(y == 0.0,
                       Vector::Zero(),
                       Vc::iif(x < 0.0,
                               Vc::exp(Vc::log(1.0 + clip(x, -1.0, 1.0)) * y),
                               1.0 - Vc::exp(Vc::log(1.0 - clip(x, -1.0, 1.0)) * y)));
    });
#else
    transform(getDrive(), [](Sample x, Sample y) {
        if (y == 0.0) {
            return 0.0;
        } else {
            x = clip(x, -1.0, 1.0);
            return x < 0.0 ? pow(1.0 + x, y) - 1.0 : 1.0 - pow(1.0 - x, y);
        }
    });
#endif
}
