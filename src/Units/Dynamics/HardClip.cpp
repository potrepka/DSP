#include "HardClip.h"

dsp::HardClip::HardClip(Space space) : Processor(Type::BIPOLAR, Type::BIPOLAR, space) {}

void dsp::HardClip::process() {
    Unit::process();
    transform([](DSP_FLOAT x) { return clip(x, -1.0, 1.0); });
}
