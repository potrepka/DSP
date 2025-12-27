// ========== Primitive Types ==========

/** C++ Sample */
export type Sample = number

/** C++ std::vector<Sample> */
export type Array = Float64Array

/** C++ std::vector<uint8_t> */
export type ByteArray = Uint8Array

// ========== FFT Types ==========

/**
 * FFT complex data
 */
export type ComplexData = {
  real: Array
  imaginary: Array
}

/**
 * FFT magnitude and phase data
 */
export type FFTData = {
  magnitude: Array
  phase: Array
}

// ========== Platform Types ==========

export type PlatformName = 'web' | 'ios' | 'android' | 'unknown'

export type PlatformInfo = {
  readonly isWeb: boolean
  readonly isNative: boolean
  readonly name: PlatformName
}
