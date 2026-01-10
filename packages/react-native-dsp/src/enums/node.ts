/**
 * Recorder recording mode
 */
export enum RecorderMode {
  FIXED_SINGLE = 0,
  FIXED_DOUBLE = 1,
  VARIABLE = 2,
}

/**
 * Spread polarity mode
 */
export enum SpreadMode {
  UNIPOLAR = 0,
  BIPOLAR = 1,
}

/**
 * Clipper boundary mode
 */
export enum ClipperMode {
  CLIP = 0,
  WRAP = 1,
  FOLD = 2,
}

/**
 * Shaper function mode
 */
export enum ShaperMode {
  HYPERBOLIC = 0,
  RATIONAL = 1,
}

/**
 * Biquad filter mode
 */
export enum BiquadMode {
  LOW_PASS = 0,
  HIGH_PASS = 1,
  BAND_PASS = 2,
  BAND_STOP = 3,
  LOW_SHELF = 4,
  HIGH_SHELF = 5,
  PEAK = 6,
  ALL_PASS = 7,
}

/**
 * OnePole filter mode
 */
export enum OnePoleMode {
  LOW_PASS = 0,
  HIGH_PASS = 1,
}

/**
 * Noise generation mode
 */
export enum NoiseMode {
  WHITE = 0,
  PINK = 1,
}

/**
 * Phasor wrapping mode
 */
export enum PhasorMode {
  WRAPPED = 0,
  UNBOUNDED = 1,
}

/**
 * Comparison operator mode
 */
export enum ComparisonMode {
  EQUAL = 0,
  NOT_EQUAL = 1,
  LESS_THAN = 2,
  LESS_THAN_OR_EQUAL = 3,
  GREATER_THAN = 4,
  GREATER_THAN_OR_EQUAL = 5,
}

/**
 * Hyperbolic function mode
 */
export enum HyperbolicMode {
  SINE = 0,
  COSINE = 1,
  TANGENT = 2,
}

/**
 * Trigonometric function mode
 */
export enum TrigonometricMode {
  SINE = 0,
  COSINE = 1,
  TANGENT = 2,
}
