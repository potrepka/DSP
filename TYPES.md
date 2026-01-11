# Signal Types

This document categorizes all node inputs and outputs by signal type.

## BOOLEAN

Signals that are either 0 or 1.

| Node           | Input/Output | Name       |
| -------------- | ------------ | ---------- |
| Recorder       | Input        | gate       |
| Recorder       | Input        | reset      |
| VariableDelay  | Input        | reset      |
| Envelope       | Input        | gate       |
| Envelope       | Input        | reset      |
| SamplePlayer   | Input        | gate       |
| SamplePlayer   | Input        | reset      |
| Phasor         | Input        | reset      |
| BooleanMask    | Input        | mask       |
| Comparison     | Output       | output     |
| NotGate        | Input        | input      |
| NotGate        | Output       | output     |
| ClockTrigger   | Output       | output     |
| ClockTrigger   | Input        | reset      |
| Differentiator | Output       | gate       |
| Differentiator | Input        | reset      |
| Integrator     | Input        | gate       |
| Integrator     | Input        | reset      |
| OnOff          | Input        | onTrigger  |
| OnOff          | Input        | offTrigger |
| OnOff          | Output       | output     |
| ResetTrigger   | Output       | output     |
| SampleAndHold  | Input        | gate       |
| SampleAndHold  | Input        | reset      |
| TriggerHold    | Input        | input      |
| TriggerHold    | Output       | output     |

## UNIPOLAR

Signals in the range [0, 1].

| Node            | Input/Output | Name      | Notes          |
| --------------- | ------------ | --------- | -------------- |
| MidSide         | Input        | mixAmount | 0=dry, 1=wet   |
| DryWet          | Input        | mixAmount | 0=dry, 1=wet   |
| Envelope        | Output       | output    | envelope level |
| TableShaper     | Input        | position  | table position |
| Phasor          | Output       | output    | phase ramp     |
| TableOscillator | Input        | phase     | 0-1 phase      |
| TableOscillator | Input        | position  | table position |

## BIPOLAR

Signals in the range [-1, 1].

| Node            | Input/Output | Name           | Notes           |
| --------------- | ------------ | -------------- | --------------- |
| ChannelMerger   | Input        | input(n)       | audio           |
| ChannelMerger   | Output       | output         | audio           |
| ChannelSplitter | Input        | input          | audio           |
| ChannelSplitter | Output       | output(n)      | audio           |
| MidSide         | Input        | input          | stereo audio    |
| MidSide         | Output       | mid            | audio           |
| MidSide         | Output       | side           | audio           |
| Spread          | Input        | input          | audio           |
| Spread          | Output       | output         | audio           |
| StereoPanner    | Input        | input          | mono audio      |
| StereoPanner    | Input        | direction      | -1=L, +1=R      |
| StereoPanner    | Output       | left           | audio           |
| StereoPanner    | Output       | right          | audio           |
| Convolver       | Input        | input          | audio           |
| Convolver       | Output       | output         | audio           |
| VariableDelay   | Input        | input          | audio           |
| VariableDelay   | Output       | output         | audio           |
| VariableDelay   | Output       | feedbackSource | audio           |
| VariableDelay   | Input        | feedbackSink   | audio           |
| CompressorGate  | Input        | input          | audio           |
| CompressorGate  | Output       | output         | audio           |
| CompressorGate  | Input        | control        | sidechain audio |
| DryWet          | Input        | dry            | audio           |
| DryWet          | Input        | wet            | audio           |
| DryWet          | Input        | a              | audio           |
| DryWet          | Input        | b              | audio           |
| DryWet          | Output       | output         | audio           |
| Shaper          | Input        | input          | audio           |
| Shaper          | Output       | output         | audio           |
| TableShaper     | Input        | input          | audio           |
| TableShaper     | Output       | output         | audio           |
| Biquad          | Input        | input          | audio           |
| Biquad          | Output       | output         | audio           |
| Crossover       | Input        | input          | audio           |
| Crossover       | Output       | low            | audio           |
| Crossover       | Output       | high           | audio           |
| OnePole         | Input        | input          | audio           |
| OnePole         | Output       | output         | audio           |
| Noise           | Output       | output         | audio           |
| SamplePlayer    | Output       | output         | audio           |
| TableOscillator | Output       | output         | audio           |
| InverseFFT      | Output       | output         | audio           |
| Recorder        | Input        | input          | audio           |

## INTEGER

Whole number signals.

| Node         | Input/Output | Name          | Notes        |
| ------------ | ------------ | ------------- | ------------ |
| Sequencer    | Input        | sequenceIndex | buffer index |
| Sequencer    | Input        | positionIndex | step index   |
| SamplePlayer | Input        | sampleIndex   | buffer index |

## SECONDS

Time duration signals.

| Node           | Input/Output | Name      |
| -------------- | ------------ | --------- |
| VariableDelay  | Input        | delayTime |
| VariableDelay  | Input        | decayTime |
| Envelope       | Input        | attack    |
| Envelope       | Input        | release   |
| CompressorGate | Input        | attack    |
| CompressorGate | Input        | release   |
| Lag            | Input        | lagTime   |
| ClockTrigger   | Input        | interval  |
| ClockTrigger   | Input        | delayTime |
| TriggerHold    | Input        | holdTime  |
| SamplePlayer   | Input        | startTime |
| BufferDuration | Output       | output    |
| SampleDuration | Output       | output    |

## HERTZ

Frequency signals.

| Node            | Input/Output | Name            |
| --------------- | ------------ | --------------- |
| Phasor          | Input        | frequency       |
| Biquad          | Input        | frequency       |
| Crossover       | Input        | frequency       |
| OnePole         | Input        | frequency       |
| FrequencyToNote | Input        | input           |
| FrequencyToNote | Input        | tuningFrequency |
| NoteToFrequency | Output       | output          |
| NoteToFrequency | Input        | tuningFrequency |
| SampleRate      | Output       | output          |
| BufferRate      | Output       | output          |

## RATIO

Arbitrary floating-point signals that don't fit the above categories.

| Node            | Input/Output | Name             | Notes                                 |
| --------------- | ------------ | ---------------- | ------------------------------------- |
| Clipper         | Input        | minimum          | arbitrary clipping bounds             |
| Clipper         | Input        | maximum          | arbitrary clipping bounds             |
| Clipper         | Input        | input            | any signal being clipped              |
| Clipper         | Output       | output           | clipped signal                        |
| CompressorGate  | Input        | threshold        | dB, unbounded negative                |
| CompressorGate  | Input        | softness         | knee width, arbitrary                 |
| CompressorGate  | Input        | compressionRatio | 1:1 to infinity                       |
| CompressorGate  | Input        | gateRatio        | 1:1 to infinity                       |
| CompressorGate  | Input        | gain             | dB, any value                         |
| Shaper          | Input        | drive            | arbitrary positive                    |
| Biquad          | Input        | resonance        | Q factor, 0.1 to 100+                 |
| Biquad          | Input        | amplitude        | dB gain for shelf/peak                |
| SamplePlayer    | Input        | speed            | 0.5=half, 2.0=double                  |
| Spread          | Input        | spread           | stereo width factor                   |
| MidiInput       | Output       | output           | MIDI values (0-127, pitch bend, etc.) |
| MidiOutput      | Input        | input            | MIDI values                           |
| ForwardFFT      | Input        | input            | time-domain signal                    |
| ForwardFFT      | Output       | magnitude        | FFT bins, unbounded                   |
| ForwardFFT      | Output       | phase            | radians, unbounded                    |
| InverseFFT      | Input        | magnitude        | FFT bins                              |
| InverseFFT      | Input        | phase            | radians                               |
| FrequencyToNote | Output       | output           | MIDI note (fractional, 0-127+)        |
| NoteToFrequency | Input        | input            | MIDI note                             |
| Division        | Input        | input            | any                                   |
| Division        | Input        | divisor          | any                                   |
| Division        | Output       | output           | any                                   |
| Floor           | Input        | input            | any                                   |
| Floor           | Input        | divisor          | any                                   |
| Floor           | Output       | output           | any                                   |
| Modulo          | Input        | input            | any                                   |
| Modulo          | Input        | divisor          | any                                   |
| Modulo          | Output       | output           | any                                   |
| Multiplication  | Input        | input            | any                                   |
| Multiplication  | Input        | factor           | any                                   |
| Multiplication  | Output       | output           | any                                   |
| Logarithm       | Input        | input            | positive values                       |
| Logarithm       | Input        | base             | log base (2, 10, e)                   |
| Logarithm       | Output       | output           | any                                   |
| Power           | Input        | input            | any                                   |
| Power           | Input        | exponent         | any                                   |
| Power           | Output       | output           | any                                   |
| Reciprocal      | Input        | input            | any                                   |
| Reciprocal      | Output       | output           | any                                   |
| Negative        | Input        | input            | any                                   |
| Negative        | Output       | output           | any                                   |
| AbsoluteValue   | Input        | input            | any                                   |
| AbsoluteValue   | Output       | output           | positive any                          |
| Hyperbolic      | Input        | input            | any                                   |
| Hyperbolic      | Output       | output           | any                                   |
| Trigonometric   | Input        | input            | radians (unbounded)                   |
| Trigonometric   | Output       | output           | depends on function                   |
| Comparison      | Input        | input            | any                                   |
| Comparison      | Input        | threshold        | any                                   |
| Function        | Input        | a                | any                                   |
| Function        | Input        | b                | any                                   |
| Function        | Output       | output           | any                                   |
| Identity        | Input        | input            | pass-through                          |
| Identity        | Output       | output           | pass-through                          |
| BooleanMask     | Input        | input            | any (masked)                          |
| BooleanMask     | Output       | output           | any                                   |
| SampleAndHold   | Input        | input            | any                                   |
| SampleAndHold   | Output       | output           | any                                   |
| Differentiator  | Input        | input            | any                                   |
| Differentiator  | Output       | output           | rate of change                        |
| Integrator      | Input        | input            | any                                   |
| Integrator      | Output       | output           | accumulated                           |
| Sequencer       | Output       | output           | sequence values                       |
| Lag             | Input        | input            | any                                   |
| Lag             | Output       | output           | any                                   |

## Summary

RATIO is necessary as a catch-all type. The math nodes (Division, Multiplication, Power, Logarithm, etc.) operate on arbitrary values by design. Additionally:

1. **dB values** (threshold, gain, amplitude) are unbounded negative/positive
2. **Ratios** (compression ratio, speed, Q factor) are arbitrary positive floats
3. **Radians** (phase, trig input) are unbounded
4. **FFT data** (magnitude, phase bins) are arbitrary
5. **Generic pass-through** (Identity, Function, SampleAndHold) must accept anything

The complete taxonomy:

- **BOOLEAN** - 0 or 1
- **UNIPOLAR** - [0, 1]
- **BIPOLAR** - [-1, 1]
- **INTEGER** - whole numbers
- **SECONDS** - time duration
- **HERTZ** - frequency
- **RATIO** - everything else (catch-all for arbitrary float)
