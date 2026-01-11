# Signal Types

This document categorizes all node inputs and outputs by signal type.

## Type Definitions

- **UNIPOLAR** - [0, 1], where 1 is a recommended maximum
- **BIPOLAR** - [-1, 1]
- **BOOLEAN** - 0 or 1
- **INTEGER** - whole numbers
- **SECONDS** - time duration
- **HERTZ** - frequency
- **NUMBER** - defined meaning that doesn't fit other categories
- **arbitrary** - constructor-defined type (can be any type)

## Complete Node Reference

| Node            | Input/Output | Name             | Type      | Notes                    |
| --------------- | ------------ | ---------------- | --------- | ------------------------ |
| AbsoluteValue   | Input        | input            | arbitrary |                          |
| AbsoluteValue   | Output       | output           | arbitrary |                          |
| Biquad          | Input        | amplitude        | UNIPOLAR  | linear gain              |
| Biquad          | Input        | frequency        | HERTZ     |                          |
| Biquad          | Input        | input            | BIPOLAR   | audio                    |
| Biquad          | Input        | resonance        | NUMBER    | (0.0, inf), default: 1.0 |
| Biquad          | Output       | output           | BIPOLAR   | audio                    |
| BooleanMask     | Input        | input            | arbitrary |                          |
| BooleanMask     | Input        | mask             | BOOLEAN   |                          |
| BooleanMask     | Output       | output           | arbitrary |                          |
| BufferDuration  | Output       | output           | SECONDS   |                          |
| BufferRate      | Output       | output           | HERTZ     |                          |
| ChannelMerger   | Input        | input(n)         | arbitrary |                          |
| ChannelMerger   | Output       | output           | arbitrary |                          |
| ChannelSplitter | Input        | input            | arbitrary |                          |
| ChannelSplitter | Output       | output(n)        | arbitrary |                          |
| Clipper         | Input        | input            | arbitrary |                          |
| Clipper         | Input        | maximum          | arbitrary | clipping bound           |
| Clipper         | Input        | minimum          | arbitrary | clipping bound           |
| Clipper         | Output       | output           | arbitrary |                          |
| ClockTrigger    | Input        | delayTime        | SECONDS   |                          |
| ClockTrigger    | Input        | interval         | SECONDS   |                          |
| ClockTrigger    | Input        | reset            | BOOLEAN   |                          |
| ClockTrigger    | Output       | output           | BOOLEAN   |                          |
| Comparison      | Input        | input            | arbitrary |                          |
| Comparison      | Input        | threshold        | arbitrary | same type as input       |
| Comparison      | Output       | output           | BOOLEAN   |                          |
| CompressorGate  | Input        | attack           | SECONDS   |                          |
| CompressorGate  | Input        | compressionRatio | NUMBER    | (0.0, inf], default: 1.0 |
| CompressorGate  | Input        | control          | BIPOLAR   | sidechain audio          |
| CompressorGate  | Input        | gain             | UNIPOLAR  | linear                   |
| CompressorGate  | Input        | gateRatio        | NUMBER    | (0.0, inf], default: 1.0 |
| CompressorGate  | Input        | input            | BIPOLAR   | audio                    |
| CompressorGate  | Input        | release          | SECONDS   |                          |
| CompressorGate  | Input        | softness         | NUMBER    | [0.0, inf], default: 0.0 |
| CompressorGate  | Input        | threshold        | UNIPOLAR  | linear                   |
| CompressorGate  | Output       | output           | BIPOLAR   | audio                    |
| Convolver       | Input        | input            | BIPOLAR   | audio                    |
| Convolver       | Output       | output           | BIPOLAR   | audio                    |
| Crossover       | Input        | frequency        | HERTZ     |                          |
| Crossover       | Input        | input            | BIPOLAR   | audio                    |
| Crossover       | Output       | high             | BIPOLAR   | audio                    |
| Crossover       | Output       | low              | BIPOLAR   | audio                    |
| Differentiator  | Input        | input            | arbitrary |                          |
| Differentiator  | Input        | reset            | BOOLEAN   |                          |
| Differentiator  | Output       | gate             | BOOLEAN   |                          |
| Differentiator  | Output       | output           | NUMBER    | derivative               |
| Division        | Input        | divisor          | arbitrary |                          |
| Division        | Input        | input            | arbitrary |                          |
| Division        | Output       | output           | arbitrary |                          |
| DryWet          | Input        | dry              | arbitrary |                          |
| DryWet          | Input        | mixAmount        | UNIPOLAR  | 0=dry, 1=wet             |
| DryWet          | Input        | wet              | arbitrary |                          |
| DryWet          | Output       | a                | arbitrary |                          |
| DryWet          | Output       | b                | arbitrary |                          |
| DryWet          | Output       | output           | arbitrary |                          |
| Envelope        | Input        | attack           | SECONDS   |                          |
| Envelope        | Input        | gate             | BOOLEAN   |                          |
| Envelope        | Input        | release          | SECONDS   |                          |
| Envelope        | Input        | reset            | BOOLEAN   |                          |
| Envelope        | Output       | output           | UNIPOLAR  | envelope level           |
| Floor           | Input        | divisor          | arbitrary |                          |
| Floor           | Input        | input            | arbitrary |                          |
| Floor           | Output       | output           | arbitrary |                          |
| ForwardFFT      | Input        | input            | BIPOLAR   | audio                    |
| ForwardFFT      | Output       | magnitude        | NUMBER    | FFT bins                 |
| ForwardFFT      | Output       | phase            | NUMBER    | radians                  |
| FrequencyToNote | Input        | input            | HERTZ     |                          |
| FrequencyToNote | Input        | tuningFrequency  | HERTZ     |                          |
| FrequencyToNote | Output       | output           | NUMBER    | MIDI note                |
| Function        | Input        | a                | arbitrary |                          |
| Function        | Input        | b                | arbitrary |                          |
| Function        | Output       | output           | arbitrary |                          |
| Hyperbolic      | Input        | input            | NUMBER    |                          |
| Hyperbolic      | Output       | output           | NUMBER    |                          |
| Identity        | Input        | input            | arbitrary |                          |
| Identity        | Output       | output           | arbitrary |                          |
| Integrator      | Input        | gate             | BOOLEAN   |                          |
| Integrator      | Input        | input            | arbitrary |                          |
| Integrator      | Input        | reset            | BOOLEAN   |                          |
| Integrator      | Output       | output           | NUMBER    | accumulated              |
| InverseFFT      | Input        | magnitude        | NUMBER    | FFT bins                 |
| InverseFFT      | Input        | phase            | NUMBER    | radians                  |
| InverseFFT      | Output       | output           | BIPOLAR   | audio                    |
| Lag             | Input        | input            | arbitrary |                          |
| Lag             | Input        | lagTime          | SECONDS   |                          |
| Lag             | Output       | output           | arbitrary |                          |
| Logarithm       | Input        | base             | NUMBER    | log base                 |
| Logarithm       | Input        | input            | NUMBER    | positive values          |
| Logarithm       | Output       | output           | NUMBER    |                          |
| MidiInput       | Output       | output           | arbitrary |                          |
| MidiOutput      | Input        | input            | arbitrary |                          |
| MidSide         | Input        | input            | arbitrary |                          |
| MidSide         | Input        | mixAmount        | UNIPOLAR  | 0=dry, 1=wet             |
| MidSide         | Output       | mid              | arbitrary |                          |
| MidSide         | Output       | side             | arbitrary |                          |
| Modulo          | Input        | divisor          | arbitrary |                          |
| Modulo          | Input        | input            | arbitrary |                          |
| Modulo          | Output       | output           | arbitrary |                          |
| Multiplication  | Input        | factor           | arbitrary |                          |
| Multiplication  | Input        | input            | arbitrary |                          |
| Multiplication  | Output       | output           | arbitrary |                          |
| Negative        | Input        | input            | arbitrary |                          |
| Negative        | Output       | output           | arbitrary |                          |
| Noise           | Output       | output           | BIPOLAR   | audio                    |
| NoteToFrequency | Input        | input            | NUMBER    | MIDI note                |
| NoteToFrequency | Input        | tuningFrequency  | HERTZ     |                          |
| NoteToFrequency | Output       | output           | HERTZ     |                          |
| NotGate         | Input        | input            | BOOLEAN   |                          |
| NotGate         | Output       | output           | BOOLEAN   |                          |
| OnePole         | Input        | frequency        | HERTZ     |                          |
| OnePole         | Input        | input            | arbitrary |                          |
| OnePole         | Output       | output           | arbitrary |                          |
| OnOff           | Input        | offTrigger       | BOOLEAN   |                          |
| OnOff           | Input        | onTrigger        | BOOLEAN   |                          |
| OnOff           | Output       | output           | BOOLEAN   |                          |
| Phasor          | Input        | frequency        | HERTZ     |                          |
| Phasor          | Input        | reset            | BOOLEAN   |                          |
| Phasor          | Output       | output           | UNIPOLAR  | phase ramp               |
| Power           | Input        | exponent         | NUMBER    |                          |
| Power           | Input        | input            | NUMBER    |                          |
| Power           | Output       | output           | NUMBER    |                          |
| Reciprocal      | Input        | input            | arbitrary |                          |
| Reciprocal      | Output       | output           | arbitrary |                          |
| Recorder        | Input        | gate             | BOOLEAN   |                          |
| Recorder        | Input        | input            | arbitrary |                          |
| Recorder        | Input        | reset            | BOOLEAN   |                          |
| ResetTrigger    | Output       | output           | BOOLEAN   |                          |
| SampleAndHold   | Input        | gate             | BOOLEAN   |                          |
| SampleAndHold   | Input        | input            | arbitrary |                          |
| SampleAndHold   | Input        | reset            | BOOLEAN   |                          |
| SampleAndHold   | Output       | output           | arbitrary |                          |
| SampleDuration  | Output       | output           | SECONDS   |                          |
| SamplePlayer    | Input        | gate             | BOOLEAN   |                          |
| SamplePlayer    | Input        | reset            | BOOLEAN   |                          |
| SamplePlayer    | Input        | sampleIndex      | INTEGER   | buffer index             |
| SamplePlayer    | Input        | speed            | NUMBER    | [0.0, inf), default: 1.0 |
| SamplePlayer    | Input        | startTime        | SECONDS   |                          |
| SamplePlayer    | Output       | output           | arbitrary |                          |
| SampleRate      | Output       | output           | HERTZ     |                          |
| Sequencer       | Input        | positionIndex    | INTEGER   | step index               |
| Sequencer       | Input        | sequenceIndex    | INTEGER   | buffer index             |
| Sequencer       | Output       | output           | arbitrary |                          |
| Shaper          | Input        | drive            | NUMBER    | [0.0, inf), default: 0.0 |
| Shaper          | Input        | input            | BIPOLAR   | audio                    |
| Shaper          | Output       | output           | BIPOLAR   | audio                    |
| Spread          | Input        | input            | arbitrary |                          |
| Spread          | Input        | spread           | arbitrary | stereo width factor      |
| Spread          | Output       | output           | arbitrary |                          |
| StereoPanner    | Input        | direction        | BIPOLAR   | -1=L, +1=R               |
| StereoPanner    | Input        | input            | arbitrary |                          |
| StereoPanner    | Output       | left             | arbitrary |                          |
| StereoPanner    | Output       | right            | arbitrary |                          |
| TableOscillator | Input        | phase            | UNIPOLAR  | 0-1 phase                |
| TableOscillator | Input        | position         | UNIPOLAR  | table position           |
| TableOscillator | Output       | output           | arbitrary |                          |
| TableShaper     | Input        | input            | BIPOLAR   | table lookup             |
| TableShaper     | Input        | position         | UNIPOLAR  | table position           |
| TableShaper     | Output       | output           | arbitrary |                          |
| TriggerHold     | Input        | holdTime         | SECONDS   |                          |
| TriggerHold     | Input        | input            | BOOLEAN   |                          |
| TriggerHold     | Output       | output           | BOOLEAN   |                          |
| Trigonometric   | Input        | input            | NUMBER    | radians                  |
| Trigonometric   | Output       | output           | NUMBER    |                          |
| VariableDelay   | Input        | decayTime        | SECONDS   |                          |
| VariableDelay   | Input        | delayTime        | SECONDS   |                          |
| VariableDelay   | Input        | feedbackSink     | arbitrary |                          |
| VariableDelay   | Input        | input            | arbitrary |                          |
| VariableDelay   | Input        | reset            | BOOLEAN   |                          |
| VariableDelay   | Output       | feedbackSource   | arbitrary |                          |
| VariableDelay   | Output       | output           | arbitrary |                          |

## Summary

The **arbitrary** type is necessary for nodes with constructor-defined types, allowing flexibility in signal routing. The **NUMBER** type serves as a catch-all for defined numeric values that don't fit other categories:

1. **Ratios** (compression ratio, Q factor, playback speed)
2. **Radians** (trigonometric input, FFT phase)
3. **FFT data** (magnitude bins)
4. **Derivatives** (Differentiator output)
5. **Accumulated values** (Integrator output)
6. **MIDI notes** (FrequencyToNote output, NoteToFrequency input)

The complete taxonomy:

- **UNIPOLAR** - [0, 1], where 1 is a recommended maximum
- **BIPOLAR** - [-1, 1]
- **BOOLEAN** - 0 or 1
- **INTEGER** - whole numbers
- **SECONDS** - time duration
- **HERTZ** - frequency
- **NUMBER** - defined meaning that doesn't fit other categories
- **arbitrary** - constructor-defined type (can be any type)
