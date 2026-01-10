import { Space, Type } from '../enums/global'
import { AudioModule, Deletable, ObjectType, Options } from '../types/module'

export const createObject = <T extends ObjectType>(
  module: AudioModule,
  objectType: T,
  options: Options<T> = {} as Options<T>,
) => {
  let reference: Deletable
  switch (objectType) {
    // Core Classes
    case 'Data': {
      const { numChannels, numSamples } = options as Options<'Data'>
      reference = new module.Data(numChannels ?? 0, numSamples ?? 0)
      break
    }
    case 'Wrapper': {
      const { data } = options as Options<'Wrapper'>
      if (data !== undefined) {
        reference = new module.Wrapper(data)
      } else {
        reference = new module.Wrapper()
      }
      break
    }
    case 'Buffer': {
      const { type, space, range, defaultValue, numChannels, numSamples } =
        options as Options<'Buffer'>
      reference = new module.Buffer(
        type ?? Type.RATIO,
        space ?? Space.TIME,
        range ?? 0,
        defaultValue ?? 0,
        numChannels ?? 0,
        numSamples ?? 0,
      )
      break
    }
    case 'Input': {
      const { type, space, range, defaultValue, numChannels, numSamples } =
        options as Options<'Input'>
      reference = new module.Input(
        type ?? Type.RATIO,
        space ?? Space.TIME,
        range ?? 0,
        defaultValue ?? 0,
        numChannels ?? 0,
        numSamples ?? 0,
      )
      break
    }
    case 'Output': {
      const { type, space, range, defaultValue, numChannels, numSamples } =
        options as Options<'Output'>
      reference = new module.Output(
        type ?? Type.RATIO,
        space ?? Space.TIME,
        range ?? 0,
        defaultValue ?? 0,
        numChannels ?? 0,
        numSamples ?? 0,
      )
      break
    }
    case 'Lockable':
      reference = new module.Lockable()
      break
    case 'Node':
      reference = new module.Node()
      break
    case 'NodeProcessor': {
      const { numInputChannels, numOutputChannels, numSamples, sampleRate } =
        options as Options<'NodeProcessor'>
      reference = new module.NodeProcessor(
        numInputChannels ?? 0,
        numOutputChannels ?? 0,
        numSamples ?? 0,
        sampleRate ?? 48000,
      )
      break
    }
    case 'NormalizedFFT':
      reference = new module.NormalizedFFT()
      break

    // Midi Classes
    case 'MidiBuffer':
      reference = new module.MidiBuffer()
      break
    case 'MidiMessage': {
      const { byte0, byte1, byte2 } = options as Options<'MidiMessage'>
      if (byte2 !== undefined) {
        reference = new module.MidiMessage(byte0 ?? 0, byte1 ?? 0, byte2)
      } else if (byte1 !== undefined) {
        reference = new module.MidiMessage(byte0 ?? 0, byte1)
      } else {
        reference = new module.MidiMessage(byte0 ?? 0)
      }
      break
    }

    // Analyzer Nodes
    case 'Recorder': {
      const { type, space, defaultValue } = options as Options<'Recorder'>
      reference = new module.Recorder(
        type ?? Type.RATIO,
        space ?? Space.TIME,
        defaultValue ?? 0,
      )
      break
    }

    // Channel Nodes
    case 'ChannelMerger': {
      const { type, space } = options as Options<'ChannelMerger'>
      reference = new module.ChannelMerger(
        type ?? Type.RATIO,
        space ?? Space.TIME,
      )
      break
    }
    case 'ChannelSplitter': {
      const { type, space } = options as Options<'ChannelSplitter'>
      reference = new module.ChannelSplitter(
        type ?? Type.RATIO,
        space ?? Space.TIME,
      )
      break
    }
    case 'MidSide': {
      const { type, space } = options as Options<'MidSide'>
      reference = new module.MidSide(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Spread': {
      const { type, space } = options as Options<'Spread'>
      reference = new module.Spread(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'StereoPanner': {
      const { type, space } = options as Options<'StereoPanner'>
      reference = new module.StereoPanner(
        type ?? Type.RATIO,
        space ?? Space.TIME,
      )
      break
    }

    // Delay Nodes
    case 'Convolver':
      reference = new module.Convolver()
      break
    case 'VariableDelay': {
      const { type } = options as Options<'VariableDelay'>
      reference = new module.VariableDelay(type ?? Type.RATIO)
      break
    }

    // Dynamics Nodes
    case 'Clipper': {
      const { type, space } = options as Options<'Clipper'>
      reference = new module.Clipper(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'CompressorGate':
      reference = new module.CompressorGate()
      break
    case 'DryWet': {
      const { type, space } = options as Options<'DryWet'>
      reference = new module.DryWet(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Envelope':
      reference = new module.Envelope()
      break
    case 'Lag': {
      const { type } = options as Options<'Lag'>
      reference = new module.Lag(type ?? Type.RATIO)
      break
    }
    case 'Shaper': {
      const { space } = options as Options<'Shaper'>
      reference = new module.Shaper(space ?? Space.TIME)
      break
    }

    // External Nodes
    case 'MidiInput': {
      const { midiBuffer, type } = options as Options<'MidiInput'>
      if (midiBuffer === undefined) {
        throw new Error('midiBuffer is required for MidiInput')
      }
      reference = new module.MidiInput(midiBuffer, type ?? Type.RATIO)
      break
    }
    case 'MidiOutput': {
      const { midiBuffer, type } = options as Options<'MidiOutput'>
      if (midiBuffer === undefined) {
        throw new Error('midiBuffer is required for MidiOutput')
      }
      reference = new module.MidiOutput(midiBuffer, type ?? Type.RATIO)
      break
    }

    // Filter Nodes
    case 'Biquad':
      reference = new module.Biquad()
      break
    case 'Crossover':
      reference = new module.Crossover()
      break
    case 'OnePole': {
      const { type } = options as Options<'OnePole'>
      reference = new module.OnePole(type ?? Type.RATIO)
      break
    }

    // Generator Nodes
    case 'Noise':
      reference = new module.Noise()
      break
    case 'Phasor':
      reference = new module.Phasor()
      break
    case 'SamplePlayer': {
      const { type } = options as Options<'SamplePlayer'>
      reference = new module.SamplePlayer(type ?? Type.RATIO)
      break
    }
    case 'TableOscillator': {
      const { type } = options as Options<'TableOscillator'>
      reference = new module.TableOscillator(type ?? Type.RATIO)
      break
    }

    // Math Nodes
    case 'AbsoluteValue': {
      const { type, space } = options as Options<'AbsoluteValue'>
      reference = new module.AbsoluteValue(
        type ?? Type.RATIO,
        space ?? Space.TIME,
      )
      break
    }
    case 'BooleanMask': {
      const { type, space } = options as Options<'BooleanMask'>
      reference = new module.BooleanMask(
        type ?? Type.RATIO,
        space ?? Space.TIME,
      )
      break
    }
    case 'Comparison': {
      const { type, space } = options as Options<'Comparison'>
      reference = new module.Comparison(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Division': {
      const { type, space } = options as Options<'Division'>
      reference = new module.Division(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Floor': {
      const { type, space } = options as Options<'Floor'>
      reference = new module.Floor(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'ForwardFFT':
      reference = new module.ForwardFFT()
      break
    case 'FrequencyToNote': {
      const { space } = options as Options<'FrequencyToNote'>
      reference = new module.FrequencyToNote(space ?? Space.TIME)
      break
    }
    case 'Function': {
      const { type, space, aType, bType, outputType } =
        options as Options<'Function'>
      if (
        aType !== undefined ||
        bType !== undefined ||
        outputType !== undefined
      ) {
        reference = new module.Function(
          aType ?? type ?? Type.RATIO,
          bType ?? type ?? Type.RATIO,
          outputType ?? type ?? Type.RATIO,
          space ?? Space.TIME,
        )
      } else {
        reference = new module.Function(type ?? Type.RATIO, space ?? Space.TIME)
      }
      break
    }
    case 'Hyperbolic': {
      const { space } = options as Options<'Hyperbolic'>
      reference = new module.Hyperbolic(space ?? Space.TIME)
      break
    }
    case 'Identity': {
      const { type, space, inputType, outputType, inputSpace, outputSpace } =
        options as Options<'Identity'>
      if (
        (inputType !== undefined || outputType !== undefined) &&
        (inputSpace !== undefined || outputSpace !== undefined)
      ) {
        reference = new module.Identity(
          inputType ?? type ?? Type.RATIO,
          outputType ?? type ?? Type.RATIO,
          inputSpace ?? space ?? Space.TIME,
          outputSpace ?? space ?? Space.TIME,
        )
      } else if (inputType !== undefined || outputType !== undefined) {
        reference = new module.Identity(
          inputType ?? type ?? Type.RATIO,
          outputType ?? type ?? Type.RATIO,
          space ?? Space.TIME,
        )
      } else {
        reference = new module.Identity(type ?? Type.RATIO, space ?? Space.TIME)
      }
      break
    }
    case 'InverseFFT':
      reference = new module.InverseFFT()
      break
    case 'Logarithm': {
      const { space } = options as Options<'Logarithm'>
      reference = new module.Logarithm(space ?? Space.TIME)
      break
    }
    case 'Modulo': {
      const { type, space } = options as Options<'Modulo'>
      reference = new module.Modulo(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Multiplication': {
      const { type, space } = options as Options<'Multiplication'>
      reference = new module.Multiplication(
        type ?? Type.RATIO,
        space ?? Space.TIME,
      )
      break
    }
    case 'Negative': {
      const { type, space } = options as Options<'Negative'>
      reference = new module.Negative(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'NoteToFrequency': {
      const { space } = options as Options<'NoteToFrequency'>
      reference = new module.NoteToFrequency(space ?? Space.TIME)
      break
    }
    case 'NotGate': {
      const { space } = options as Options<'NotGate'>
      reference = new module.NotGate(space ?? Space.TIME)
      break
    }
    case 'Power': {
      const { space } = options as Options<'Power'>
      reference = new module.Power(space ?? Space.TIME)
      break
    }
    case 'Reciprocal': {
      const { type, space } = options as Options<'Reciprocal'>
      reference = new module.Reciprocal(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Trigonometric': {
      const { space } = options as Options<'Trigonometric'>
      reference = new module.Trigonometric(space ?? Space.TIME)
      break
    }

    // Trigger Nodes
    case 'ClockTrigger':
      reference = new module.ClockTrigger()
      break
    case 'Differentiator': {
      const { type } = options as Options<'Differentiator'>
      reference = new module.Differentiator(type ?? Type.RATIO)
      break
    }
    case 'Integrator': {
      const { type } = options as Options<'Integrator'>
      reference = new module.Integrator(type ?? Type.RATIO)
      break
    }
    case 'OnOff':
      reference = new module.OnOff()
      break
    case 'ResetTrigger':
      reference = new module.ResetTrigger()
      break
    case 'SampleAndHold': {
      const { type } = options as Options<'SampleAndHold'>
      reference = new module.SampleAndHold(type ?? Type.RATIO)
      break
    }
    case 'Sequencer': {
      const { type, space } = options as Options<'Sequencer'>
      reference = new module.Sequencer(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'TriggerHold':
      reference = new module.TriggerHold()
      break

    // Variable Nodes
    case 'BufferDuration':
      reference = new module.BufferDuration()
      break
    case 'BufferRate':
      reference = new module.BufferRate()
      break
    case 'SampleDuration':
      reference = new module.SampleDuration()
      break
    case 'SampleRate':
      reference = new module.SampleRate()
      break

    default:
      throw new Error(`Unsupported object type: ${objectType}`)
  }
  return reference
}
