#pragma once

namespace dsp {

typedef float Sample;

const Sample PI = 3.141592653589793;
const Sample TAU = 6.283185307179586;
const Sample PI_OVER_TWO = 1.570796326794897;
const Sample PHI = 1.618033988749895;
const Sample SQRT2 = 1.414213562373095;
const Sample SIX_DB = 6.020599913279624;

const Sample ONE_OVER_PI = 0.3183098861837907;
const Sample ONE_OVER_TAU = 0.1591549430918953;
const Sample TWO_OVER_PI = 0.6366197723675813;
const Sample ONE_OVER_PHI = 0.6180339887498948;
const Sample ONE_OVER_SQRT2 = 0.7071067811865475;
const Sample ONE_OVER_SIX_DB = 0.1660964047443681;

enum class Type { BIPOLAR, UNIPOLAR, SECONDS, HERTZ, RATIO, LINEAR, INTEGER, BINARY };

enum class Space { TIME, FREQUENCY };

enum class Interpolation { LINEAR, HERMITE };

} // namespace dsp
