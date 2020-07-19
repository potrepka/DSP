#include "HardClip.h"

dsp::HardClip::HardClip(Space space)
        : Processor(Type::BIPOLAR, Type::BIPOLAR, space) {}

void dsp::HardClip::process() {
    Unit::process();
    transform([](Sample x) { return clip(x, -1.0, 1.0); });
}
