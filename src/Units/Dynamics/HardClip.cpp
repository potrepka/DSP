#include "HardClip.h"

dsp::HardClip::HardClip(Connection::Space space)
        : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR, space) {}

void dsp::HardClip::process() {
    Unit::process();
    transform([](DSP_FLOAT x) { return clip(x, -1.0, 1.0); });
}
