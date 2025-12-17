/**
 * Type definitions for @potrepka/native-audio
 * Matches C++ type definitions and provides TypeScript-specific types
 */

import type { Type, Space } from './enums';

// ========== Primitive Types (matching C++ typedefs) ==========

/** Audio sample value (C++ double) */
export type Sample = number;

/** Array of samples (C++ std::vector<Sample>) */
export type Array = Float64Array | number[];

/** MIDI byte value (C++ uint8_t) */
export type Byte = number;

// ========== Audio Buffer Types ==========

/** Multi-channel audio buffer (Array of Float32Arrays) */
export type AudioBuffer = Float32Array[];

/** Single channel audio data */
export type ChannelData = Float32Array;

// ========== Configuration Types ==========

/**
 * Buffer configuration options
 */
export type BufferConfig = {
  type?: Type;
  space?: Space;
  range?: Sample;
  defaultValue?: Sample;
  numChannels?: number;
  numSamples?: number;
};

/**
 * Node configuration options
 */
export type NodeConfig = {
  numChannels?: number;
  numInputChannels?: number;
  numOutputChannels?: number;
  numSamples?: number;
  sampleRate?: number;
};

/**
 * NodeProcessor configuration options
 */
export type NodeProcessorConfig = {
  numInputChannels?: number;
  numOutputChannels?: number;
  numSamples?: number;
  sampleRate?: number;
};

// ========== FFT Types ==========

/**
 * FFT magnitude and phase data
 */
export type FFTData = {
  magnitude: Float32Array;
  phase: Float32Array;
};

/**
 * Complex number representation
 */
export type ComplexData = {
  real: Float32Array;
  imaginary: Float32Array;
};

// ========== Convolver Types ==========

/**
 * Impulse response mapping
 */
export type ImpulseResponseMap = Map<string, Float32Array>;

/**
 * Convolver configuration
 */
export type ConvolverConfig = {
  headSize: number;
  tailSize: number;
  impulseResponses?: ImpulseResponseMap;
};

// ========== MIDI Types ==========

/**
 * MIDI message data structure
 */
export type MidiMessageData = {
  bytes: Uint8Array;
  samplePosition?: number;
};

/**
 * MIDI buffer data
 */
export type MidiBufferData = {
  messages: MidiMessageData[];
};

// ========== Platform Types ==========

/**
 * Platform identifier
 */
export type Platform = 'web' | 'native' | 'unknown';

/**
 * Module instance (WASM module on Web, native module on React Native)
 */
export type ModuleInstance = any;
