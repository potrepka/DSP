#pragma once

#include "Constants.h"
#include <cmath>
#include <vector>

namespace dsp {

DSP_FLOAT clip(const DSP_FLOAT signal);
DSP_FLOAT decibelsToAmplitude(const DSP_FLOAT decibels);
DSP_FLOAT amplitudeToDecibels(const DSP_FLOAT amplitude);
DSP_FLOAT linear(std::vector<DSP_FLOAT> &table, const DSP_FLOAT index);
DSP_FLOAT hermite(std::vector<DSP_FLOAT> &table, const DSP_FLOAT index);

} // namespace dsp
