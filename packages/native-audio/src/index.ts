/**
 * @potrepka/native-audio
 * High-performance audio DSP library for TypeScript with Web and React Native support
 */

import { Platform } from './platform';

// Core Base Classes
export { Disposable } from './core/Disposable';
export { Node } from './core/Node';
export { Consumer } from './core/Consumer';
export { Producer } from './core/Producer';
export { Transformer } from './core/Transformer';
export { NodeProcessor } from './core/NodeProcessor';
export { Buffer } from './core/Buffer';
export { AudioWrapper } from './core/AudioWrapper';
export { Input } from './core/Input';
export { Output } from './core/Output';
export { GenericNode } from './core/GenericNode';

// Platform
export { Platform } from './platform';

// Enums
export * from './enums';

// Types
export * from './types';

// Core
export { Engine } from './core/Engine';
export { Lockable } from './core/Lockable';
export { NormalizedFFT } from './core/ScaledFFT';

// Midi
export { MidiMessage } from './midi/MidiMessage';
export { MidiBuffer } from './midi/MidiBuffer';
export { MidiProcessor, MidiProcessorInput, MidiProcessorOutput } from './midi/MidiProcessor';

// Nodes - Filters
export { Biquad } from './nodes/filters/Biquad';
export { OnePole } from './nodes/filters/OnePole';
export { Crossover } from './nodes/filters/Crossover';

// Nodes - Math
export { Identity } from './nodes/math/Identity';
export { Multiplication } from './nodes/math/Multiplication';
export { Division } from './nodes/math/Division';
export { AbsoluteValue } from './nodes/math/AbsoluteValue';
export { Negative } from './nodes/math/Negative';
export { Reciprocal } from './nodes/math/Reciprocal';
export { Logarithm } from './nodes/math/Logarithm';
export { Power } from './nodes/math/Power';
export { Floor } from './nodes/math/Floor';
export { Modulo } from './nodes/math/Modulo';
export { Trigonometric } from './nodes/math/Trigonometric';
export { Hyperbolic } from './nodes/math/Hyperbolic';
export { NoteToFrequency } from './nodes/math/NoteToFrequency';
export { FrequencyToNote } from './nodes/math/FrequencyToNote';
export { NotGate } from './nodes/math/NotGate';
export { BooleanMask } from './nodes/math/BooleanMask';
export { Comparison } from './nodes/math/Comparison';
export { ForwardFFT } from './nodes/math/ForwardFFT';

// Nodes - Generators
export { Phasor } from './nodes/generators/Phasor';
export { Noise } from './nodes/generators/Noise';
export { Envelope } from './nodes/generators/Envelope';
export { TableOscillator } from './nodes/generators/TableOscillator';
export { ClockTrigger } from './nodes/generators/ClockTrigger';
export { OnOff } from './nodes/generators/OnOff';
export { ResetTrigger } from './nodes/generators/ResetTrigger';
export { Sequencer } from './nodes/generators/Sequencer';
export { ChannelMerger } from './nodes/generators/ChannelMerger';
export { BufferDuration } from './nodes/generators/BufferDuration';
export { BufferRate } from './nodes/generators/BufferRate';
export { SampleDuration } from './nodes/generators/SampleDuration';
export { SampleRate } from './nodes/generators/SampleRate';
export { DryWet } from './nodes/generators/DryWet';
export { InverseFFT } from './nodes/generators/InverseFFT';

// Nodes - Dynamics
export { CompressorGate } from './nodes/dynamics/CompressorGate';
export { Clipper } from './nodes/dynamics/Clipper';
export { Lag } from './nodes/dynamics/Lag';
export { Shaper } from './nodes/dynamics/Shaper';

// Nodes - Delays
export { VariableDelay } from './nodes/delays/VariableDelay';
export { Convolver } from './nodes/delays/Convolver';

// Nodes - Triggers
export { Differentiator } from './nodes/triggers/Differentiator';
export { TriggerHold } from './nodes/triggers/TriggerHold';
export { Integrator } from './nodes/triggers/Integrator';
export { SampleAndHold } from './nodes/triggers/SampleAndHold';

// Nodes - Channels
export { Spread } from './nodes/channels/Spread';
export { ChannelSplitter } from './nodes/channels/ChannelSplitter';
export { MidSide } from './nodes/channels/MidSide';
export { StereoPanner } from './nodes/channels/StereoPanner';

// Nodes - Analyzers
export { Recorder } from './nodes/analyzers/Recorder';

/**
 * Initialize the audio library (Web only)
 * Must be called before creating any nodes on Web platform
 */
export async function initialize(): Promise<void> {
  if (Platform.isWeb) {
    // Load WASM module
    if (typeof window !== 'undefined') {
      // Check if module is already loaded
      if (typeof (window as any).Module !== 'undefined') {
        return;
      }

      // Load the module
      try {
        const createModule = (window as any).createNativeAudioModule;
        if (!createModule) {
          throw new Error('WASM module not found. Make sure native-audio.js is loaded.');
        }

        const module = await createModule();
        (window as any).Module = module;
      } catch (error) {
        throw new Error(`Failed to initialize WASM module: ${error}`);
      }
    }
  }
  // Native platform doesn't need initialization
}

/**
 * Check if the library is initialized and ready to use
 */
export function isInitialized(): boolean {
  if (Platform.isWeb) {
    return typeof (window as any).Module !== 'undefined';
  } else if (Platform.isNative) {
    return typeof (globalThis as any).NativeAudio !== 'undefined';
  }
  return false;
}
