#include "Distortion.h"

dsp::Distortion::Distortion(Space space)
        : Processor(Type::BIPOLAR, Type::BIPOLAR, space)
        , drive(pushInput(Type::RATIO, space, 1.0)) {}

std::shared_ptr<dsp::InputParameter> dsp::Distortion::getDrive() const {
    return drive;
}

void dsp::Distortion::process() {
    Unit::process();
#if DSP_USE_VC
    transform(getDrive(), [](Vector x, Vector y) {
        Vector ry = Vector::One() / y;
        Vector a = y - ry;
        Vector z;
        for (int k = 0; k < Vector::Size; k++) {
            if (x[k] < 0.0) {
                z[k] = (1.0 / (ry[k] - a[k] * x[k]) - y[k]) / a[k];
            } else {
                z[k] = (y[k] - 1.0 / (ry[k] + a[k] * x[k])) / a[k];
            }
        }
        z(!Vc::isfinite(y)) = Vc::iif(x < Vector::Zero(), -Vector::One(), Vector::One());
        z(y == Vector::One()) = x;
        z(x == Vector::Zero() || y == Vector::Zero()) = Vector::Zero();
        return z;
    });
#else
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
        Sample ry = 1.0 / y;
        Sample a = y - ry;
        if (x < 0.0) {
            return (1.0 / (ry - a * x) - y) / a;
        } else {
            return (y - 1.0 / (ry + a * x)) / a;
        }
    });
#endif
}
