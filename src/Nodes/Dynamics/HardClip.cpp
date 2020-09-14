#include "HardClip.h"

dsp::HardClip::HardClip(Space space)
        : Transformer(Type::RATIO, Type::RATIO, space) {}

void dsp::HardClip::processNoLock() {
    transform([](Sample x) { return clip(x, -1.0, 1.0); });
}
