#pragma once

#include <cmath>
#include "Connection.h"

namespace dsp {

DSP_FLOAT toAmplitude(const DSP_FLOAT decibels);
DSP_FLOAT toDecibels(const DSP_FLOAT amplitude);
DSP_FLOAT toHertz(const DSP_FLOAT seconds);
DSP_FLOAT toSeconds(const DSP_FLOAT hertz);
DSP_FLOAT linear(std::vector<DSP_FLOAT>& table, const DSP_FLOAT index);
DSP_FLOAT hermite(std::vector<DSP_FLOAT>& table, const DSP_FLOAT index);

}
