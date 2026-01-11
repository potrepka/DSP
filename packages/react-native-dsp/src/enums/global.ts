/**
 * Type of buffer value representation
 */
export enum Type {
  RATIO = 0,
  INTEGER = 1,
  BOOLEAN = 2,
  HERTZ = 3,
  SECONDS = 4,
}

/**
 * Signal processing domain
 */
export enum Domain {
  TIME = 0,
  FREQUENCY = 1,
}

/**
 * Curve shape for envelopes and transitions
 */
export enum Shape {
  LINEAR = 0,
  EXPONENTIAL = 1,
}

/**
 * Interpolation method for sample lookups
 */
export enum Interpolation {
  NONE = 0,
  LINEAR = 1,
  HERMITE = 2,
}

/**
 * Input summing mode
 */
export enum InputMode {
  SUM = 0,
  MINIMUM = 1,
  MAXIMUM = 2,
}
