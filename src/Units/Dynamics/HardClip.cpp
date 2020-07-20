#include "HardClip.h"

dsp::HardClip::HardClip(Space space)
        : Processor(Type::BIPOLAR, Type::BIPOLAR, space) {}

void dsp::HardClip::process() {
    Unit::process();
#if DSP_USE_VC
    transform([](Vector x) { return clip(x, -1.0, 1.0); });
#else
    transform([](Sample x) { return clip(x, -1.0, 1.0); });
#endif
}
