import { Domain, Type } from '../enums/global'
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
      const { type, domain, range, defaultValue, numChannels, numSamples } =
        options as Options<'Buffer'>
      reference = new module.Buffer(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
        range ?? 0,
        defaultValue ?? 0,
        numChannels ?? 0,
        numSamples ?? 0,
      )
      break
    }
    case 'Input': {
      const { type, domain, range, defaultValue, numChannels, numSamples } =
        options as Options<'Input'>
      reference = new module.Input(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
        range ?? 0,
        defaultValue ?? 0,
        numChannels ?? 0,
        numSamples ?? 0,
      )
      break
    }
    case 'Output': {
      const { type, domain, range, defaultValue, numChannels, numSamples } =
        options as Options<'Output'>
      reference = new module.Output(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
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
      const { type, domain, defaultValue } = options as Options<'Recorder'>
      reference = new module.Recorder(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
        defaultValue ?? 0,
      )
      break
    }

    // Channel Nodes
    case 'ChannelMerger': {
      const { type, domain } = options as Options<'ChannelMerger'>
      reference = new module.ChannelMerger(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
      )
      break
    }
    case 'ChannelSplitter': {
      const { type, domain } = options as Options<'ChannelSplitter'>
      reference = new module.ChannelSplitter(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
      )
      break
    }
    case 'MidSide': {
      const { type, domain } = options as Options<'MidSide'>
      reference = new module.MidSide(type ?? Type.RATIO, domain ?? Domain.TIME)
      break
    }
    case 'Spread': {
      const { type, domain } = options as Options<'Spread'>
      reference = new module.Spread(type ?? Type.RATIO, domain ?? Domain.TIME)
      break
    }
    case 'StereoPanner': {
      const { type, domain } = options as Options<'StereoPanner'>
      reference = new module.StereoPanner(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
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
      const { type, domain } = options as Options<'Clipper'>
      reference = new module.Clipper(type ?? Type.RATIO, domain ?? Domain.TIME)
      break
    }
    case 'CompressorGate':
      reference = new module.CompressorGate()
      break
    case 'DryWet': {
      const { type, domain } = options as Options<'DryWet'>
      reference = new module.DryWet(type ?? Type.RATIO, domain ?? Domain.TIME)
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
      const { domain } = options as Options<'Shaper'>
      reference = new module.Shaper(domain ?? Domain.TIME)
      break
    }
    case 'TableShaper': {
      const { outputType, domain } = options as Options<'TableShaper'>
      reference = new module.TableShaper(
        outputType ?? Type.RATIO,
        domain ?? Domain.TIME,
      )
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
      const { type, domain } = options as Options<'AbsoluteValue'>
      reference = new module.AbsoluteValue(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
      )
      break
    }
    case 'BooleanMask': {
      const { type, domain } = options as Options<'BooleanMask'>
      reference = new module.BooleanMask(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
      )
      break
    }
    case 'Comparison': {
      const { type, domain } = options as Options<'Comparison'>
      reference = new module.Comparison(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
      )
      break
    }
    case 'Division': {
      const { type, domain } = options as Options<'Division'>
      reference = new module.Division(type ?? Type.RATIO, domain ?? Domain.TIME)
      break
    }
    case 'Floor': {
      const { type, domain } = options as Options<'Floor'>
      reference = new module.Floor(type ?? Type.RATIO, domain ?? Domain.TIME)
      break
    }
    case 'ForwardFFT':
      reference = new module.ForwardFFT()
      break
    case 'FrequencyToNote': {
      const { domain } = options as Options<'FrequencyToNote'>
      reference = new module.FrequencyToNote(domain ?? Domain.TIME)
      break
    }
    case 'Function': {
      const { type, domain, aType, bType, outputType } =
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
          domain ?? Domain.TIME,
        )
      } else {
        reference = new module.Function(
          type ?? Type.RATIO,
          domain ?? Domain.TIME,
        )
      }
      break
    }
    case 'Hyperbolic': {
      const { domain } = options as Options<'Hyperbolic'>
      reference = new module.Hyperbolic(domain ?? Domain.TIME)
      break
    }
    case 'Identity': {
      const { type, domain, inputType, outputType, inputDomain, outputDomain } =
        options as Options<'Identity'>
      if (
        (inputType !== undefined || outputType !== undefined) &&
        (inputDomain !== undefined || outputDomain !== undefined)
      ) {
        reference = new module.Identity(
          inputType ?? type ?? Type.RATIO,
          outputType ?? type ?? Type.RATIO,
          inputDomain ?? domain ?? Domain.TIME,
          outputDomain ?? domain ?? Domain.TIME,
        )
      } else if (inputType !== undefined || outputType !== undefined) {
        reference = new module.Identity(
          inputType ?? type ?? Type.RATIO,
          outputType ?? type ?? Type.RATIO,
          domain ?? Domain.TIME,
        )
      } else {
        reference = new module.Identity(
          type ?? Type.RATIO,
          domain ?? Domain.TIME,
        )
      }
      break
    }
    case 'InverseFFT':
      reference = new module.InverseFFT()
      break
    case 'Logarithm': {
      const { domain } = options as Options<'Logarithm'>
      reference = new module.Logarithm(domain ?? Domain.TIME)
      break
    }
    case 'Modulo': {
      const { type, domain } = options as Options<'Modulo'>
      reference = new module.Modulo(type ?? Type.RATIO, domain ?? Domain.TIME)
      break
    }
    case 'Multiplication': {
      const { type, domain } = options as Options<'Multiplication'>
      reference = new module.Multiplication(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
      )
      break
    }
    case 'Negative': {
      const { type, domain } = options as Options<'Negative'>
      reference = new module.Negative(type ?? Type.RATIO, domain ?? Domain.TIME)
      break
    }
    case 'NoteToFrequency': {
      const { domain } = options as Options<'NoteToFrequency'>
      reference = new module.NoteToFrequency(domain ?? Domain.TIME)
      break
    }
    case 'NotGate': {
      const { domain } = options as Options<'NotGate'>
      reference = new module.NotGate(domain ?? Domain.TIME)
      break
    }
    case 'Power': {
      const { domain } = options as Options<'Power'>
      reference = new module.Power(domain ?? Domain.TIME)
      break
    }
    case 'Reciprocal': {
      const { type, domain } = options as Options<'Reciprocal'>
      reference = new module.Reciprocal(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
      )
      break
    }
    case 'Trigonometric': {
      const { domain } = options as Options<'Trigonometric'>
      reference = new module.Trigonometric(domain ?? Domain.TIME)
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
      const { type, domain } = options as Options<'Sequencer'>
      reference = new module.Sequencer(
        type ?? Type.RATIO,
        domain ?? Domain.TIME,
      )
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
