#include "Modulo.h"

dsp::Modulo::Modulo(Type type, Space space)
        : Processor(type, type, space)
        , divisor(pushInput(type, space, 1.0)) {
    assert(type != Type::BINARY);
    assert(type != Type::INTEGER);
}

std::shared_ptr<dsp::InputParameter> dsp::Modulo::getDivisor() const {
    return divisor;
}

void dsp::Modulo::process() {
    Unit::process();
    transform(getDivisor(),
#if DSP_USE_VC
              [](Vector x, Vector y) { return Vc::iif(y == 0.0, Vector::Zero(), x - Vc::floor(x / y) * y); });
#else
              [](Sample x, Sample y) { return y == 0.0 ? 0.0 : x - floor(x / y) * y; });
#endif
}
