#pragma once

#ifdef DSP_USE_JUCE
#include <JuceHeader.h>
#define DSP_ASSERT jassert
#else
#define DSP_ASSERT assert
#endif

namespace dsp {

#ifdef DSP_USE_JUCE
typedef juce::uint8 uint8;
#else
typedef uint8_t uint8;
#endif

typedef double Sample;
typedef std::vector<Sample> Array;

const Sample PI = 3.141592653589793;
const Sample ONE_OVER_PI = 0.3183098861837907;

const Sample TAU = 6.283185307179586;
const Sample ONE_OVER_TAU = 0.1591549430918953;

const Sample PI_OVER_TWO = 1.570796326794897;
const Sample SQRT_OF_TWO = 1.414213562373095;

const Sample PHI = 1.618033988749895;
const Sample E = 2.718281828459045;

enum class Type { RATIO, INTEGER, BOOLEAN, HERTZ, SECONDS };

enum class Space { TIME, FREQUENCY };

struct Shape {
    static const int MIN = 0;
    static const int MAX = 1;
    static const int LINEAR = 0;
    static const int EXPONENTIAL = 1;
};

struct Interpolation {
    static const int MIN = 0;
    static const int MAX = 2;
    static const int NONE = 0;
    static const int LINEAR = 1;
    static const int HERMITE = 2;
};

} // namespace dsp
