#pragma once

#include "Constants.h"
#include <cmath>
#include <vector>

namespace dsp {

DSP_FLOAT negative(const DSP_FLOAT value);
DSP_FLOAT oneOver(const DSP_FLOAT value);
DSP_FLOAT clip(const DSP_FLOAT signal, const DSP_FLOAT min, const DSP_FLOAT max);
DSP_FLOAT wrap(const DSP_FLOAT signal, const DSP_FLOAT min, const DSP_FLOAT max);
DSP_FLOAT toBinary(const DSP_FLOAT value);
DSP_FLOAT toInteger(const DSP_FLOAT value);
DSP_FLOAT bipolarToUnipolar(const DSP_FLOAT bipolar);
DSP_FLOAT unipolarToBipolar(const DSP_FLOAT unipolar);
DSP_FLOAT decibelsToLinear(const DSP_FLOAT decibels);
DSP_FLOAT linearToDecibels(const DSP_FLOAT linear);
DSP_FLOAT decibelsToRatio(const DSP_FLOAT decibels);
DSP_FLOAT ratioToDecibels(const DSP_FLOAT ratio);
DSP_FLOAT linear(const std::vector<DSP_FLOAT> &table, const DSP_FLOAT index, DSP_FLOAT defaultValue = 0.0);
DSP_FLOAT hermite(const std::vector<DSP_FLOAT> &table, const DSP_FLOAT index, DSP_FLOAT defaultValue = 0.0);

} // namespace dsp
