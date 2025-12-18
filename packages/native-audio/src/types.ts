import type { Space, Type } from './enums'

// ========== Primitive Types (matching C++ typedefs) ==========

/** Audio sample value (C++ double) */
export type Sample = number

/** Array of samples (C++ std::vector<Sample>) */
export type Array = Float64Array | number[]

// ========== Configuration Types ==========

/**
 * Buffer configuration options
 */
export type BufferConfig = {
  type?: Type
  space?: Space
  range?: Sample
  defaultValue?: Sample
  numChannels?: number
  numSamples?: number
}

/**
 * Node configuration options
 */
export type NodeConfig = {
  numChannels?: number
  numInputChannels?: number
  numOutputChannels?: number
  numSamples?: number
  sampleRate?: number
}

/**
 * NodeProcessor configuration options
 */
export type NodeProcessorConfig = {
  numInputChannels?: number
  numOutputChannels?: number
  numSamples?: number
  sampleRate?: number
}

// ========== FFT Types ==========

/**
 * FFT magnitude and phase data
 */
export type FFTData = {
  magnitude: Array
  phase: Array
}

/**
 * Complex number representation
 */
export type ComplexData = {
  real: Array
  imaginary: Array
}

// ========== Convolver Types ==========

/**
 * Impulse response mapping
 */
export type ImpulseResponseMap = Map<string, Array>

/**
 * Convolver configuration
 */
export type ConvolverConfig = {
  headSize: number
  tailSize: number
  impulseResponses?: ImpulseResponseMap
}

// ========== MIDI Types ==========

/**
 * MIDI message data structure
 */
export type MidiMessageData = {
  bytes: Uint8Array
  samplePosition?: number
}

/**
 * MIDI buffer data
 */
export type MidiBufferData = {
  messages: MidiMessageData[]
}

// ========== Platform Types ==========

export type PlatformName = 'web' | 'ios' | 'android' | 'unknown'

export type PlatformInfo = {
  readonly isWeb: boolean
  readonly isNative: boolean
  readonly name: PlatformName
}
