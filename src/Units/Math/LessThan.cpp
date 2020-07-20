#include "LessThan.h"

dsp::LessThan::LessThan(Type type, Space space)
        : Processor(type, Type::BINARY, space)
        , comparisonSignal(pushInput(type, space)) {}

std::shared_ptr<dsp::InputParameter> dsp::LessThan::getComparisonSignal() const {
    return comparisonSignal;
}

void dsp::LessThan::process() {
    Unit::process();
    transform(getComparisonSignal(),
#if DSP_USE_VC
              [](Vector x, Vector y) { return Vc::iif(x < y, Vector::One(), Vector::Zero()); });
#else
              [](Sample x, Sample y) { return x < y; });
#endif
}
