#pragma once

#include <algorithm>
#include <cmath>

#include "../Midi/MidiBuffer.h"
#include "Constants.h"

namespace dsp {

Sample byteToUnipolar(const int value);
int unipolarToByte(const Sample sample);
Sample shortToBipolar(const int value);
int bipolarToShort(const Sample sample);

Sample clip(const Sample sample, const Sample min, const Sample max);
Sample wrap(const Sample sample, const Sample max);

Sample linear(const Sample x1, const Sample x2, const Sample mu);
Sample hermite(const Sample x0, const Sample x1, const Sample x2,
               const Sample x3, const Sample mu);
Sample linearClipped(Sample* data, const size_t size, const Sample index,
                     const Sample defaultValue = 0.0);
Sample hermiteClipped(Sample* data, const size_t size, const Sample index,
                      const Sample defaultValue = 0.0);
Sample linearWrapped(Sample* data, const size_t size, const Sample index,
                     const Sample defaultValue = 0.0);
Sample hermiteWrapped(Sample* data, const size_t size, const Sample index,
                      const Sample defaultValue = 0.0);

} // namespace dsp
