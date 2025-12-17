#pragma once

#include "Constants.h"
#include "../Midi/MidiBuffer.h"

#include <cmath>

namespace dsp {

Sample byteToUnipolar(const int value);
int unipolarToByte(const Sample sample);
Sample shortToBipolar(const int value);
int bipolarToShort(const Sample sample);

Sample clip(const Sample sample, const Sample min, const Sample max);
Sample wrap(const Sample sample, const Sample max);

Sample linear(Sample *data, const size_t size, const Sample index, const Sample defaultValue = 0.0);
Sample hermite(Sample *data, const size_t size, const Sample index, const Sample defaultValue = 0.0);

} // namespace dsp
