import { Space, Type } from '../enums'
import { ReservedKeyword } from '../enums/module'
import { AudioModule, Node, NodeOptions, NodeType } from '../types'

export const getReservedKeywords = () =>
  Object.values(ReservedKeyword) as string[]

export const constructNode = <T extends NodeType>(
  module: AudioModule,
  nodeType: T,
  options: NodeOptions<T>,
) => {
  let node: Node
  switch (nodeType) {
    // Analyzer Nodes
    case 'Recorder': {
      const { type, space, defaultValue } = options as NodeOptions<'Recorder'>
      node = new module.Recorder(
        type ?? Type.RATIO,
        space ?? Space.TIME,
        defaultValue ?? 0,
      )
      break
    }

    // Channel Nodes
    case 'ChannelMerger': {
      const { type, space } = options as NodeOptions<'ChannelMerger'>
      node = new module.ChannelMerger(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'ChannelSplitter': {
      const { type, space } = options as NodeOptions<'ChannelSplitter'>
      node = new module.ChannelSplitter(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'MidSide': {
      const { type, space } = options as NodeOptions<'MidSide'>
      node = new module.MidSide(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Spread': {
      const { type, space } = options as NodeOptions<'Spread'>
      node = new module.Spread(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'StereoPanner': {
      const { type, space } = options as NodeOptions<'StereoPanner'>
      node = new module.StereoPanner(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }

    // Delay Nodes
    case 'Convolver':
      node = new module.Convolver()
      break
    case 'VariableDelay': {
      const { type } = options as NodeOptions<'VariableDelay'>
      node = new module.VariableDelay(type ?? Type.RATIO)
      break
    }

    // Dynamics Nodes
    case 'Clipper': {
      const { type, space } = options as NodeOptions<'Clipper'>
      node = new module.Clipper(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'CompressorGate':
      node = new module.CompressorGate()
      break
    case 'DryWet': {
      const { type, space } = options as NodeOptions<'DryWet'>
      node = new module.DryWet(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Envelope':
      node = new module.Envelope()
      break
    case 'Lag': {
      const { type } = options as NodeOptions<'Lag'>
      node = new module.Lag(type ?? Type.RATIO)
      break
    }
    case 'Shaper': {
      const { space } = options as NodeOptions<'Shaper'>
      node = new module.Shaper(space ?? Space.TIME)
      break
    }

    // External Nodes
    case 'MidiInput': {
      const { midiBuffer, type } = options as NodeOptions<'MidiInput'>
      node = new module.MidiInput(midiBuffer, type ?? Type.RATIO)
      break
    }
    case 'MidiOutput': {
      const { midiBuffer, type } = options as NodeOptions<'MidiOutput'>
      node = new module.MidiOutput(midiBuffer, type ?? Type.RATIO)
      break
    }

    // Filter Nodes
    case 'Biquad':
      node = new module.Biquad()
      break
    case 'Crossover':
      node = new module.Crossover()
      break
    case 'OnePole': {
      const { type } = options as NodeOptions<'OnePole'>
      node = new module.OnePole(type ?? Type.RATIO)
      break
    }

    // Generator Nodes
    case 'FunctionOscillator': {
      const { type } = options as NodeOptions<'FunctionOscillator'>
      node = new module.FunctionOscillator(type ?? Type.RATIO)
      break
    }
    case 'MoorerOscillator':
      node = new module.MoorerOscillator()
      break
    case 'Noise':
      node = new module.Noise()
      break
    case 'Phasor':
      node = new module.Phasor()
      break
    case 'SamplePlayer': {
      const { type } = options as NodeOptions<'SamplePlayer'>
      node = new module.SamplePlayer(type ?? Type.RATIO)
      break
    }
    case 'TableOscillator': {
      const { type } = options as NodeOptions<'TableOscillator'>
      node = new module.TableOscillator(type ?? Type.RATIO)
      break
    }

    // Math Nodes
    case 'AbsoluteValue': {
      const { type, space } = options as NodeOptions<'AbsoluteValue'>
      node = new module.AbsoluteValue(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'BooleanMask': {
      const { type, space } = options as NodeOptions<'BooleanMask'>
      node = new module.BooleanMask(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Comparison': {
      const { type, space } = options as NodeOptions<'Comparison'>
      node = new module.Comparison(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Division': {
      const { type, space } = options as NodeOptions<'Division'>
      node = new module.Division(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Floor': {
      const { type, space } = options as NodeOptions<'Floor'>
      node = new module.Floor(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'ForwardFFT':
      node = new module.ForwardFFT()
      break
    case 'FrequencyToNote': {
      const { space } = options as NodeOptions<'FrequencyToNote'>
      node = new module.FrequencyToNote(space ?? Space.TIME)
      break
    }
    case 'Hyperbolic': {
      const { space } = options as NodeOptions<'Hyperbolic'>
      node = new module.Hyperbolic(space ?? Space.TIME)
      break
    }
    case 'Identity': {
      const { type, space, inputType, outputType, inputSpace, outputSpace } =
        options as NodeOptions<'Identity'>
      if ((inputType || outputType) && (inputSpace || outputSpace)) {
        node = new module.Identity(
          inputType ?? type ?? Type.RATIO,
          outputType ?? type ?? Type.RATIO,
          inputSpace ?? space ?? Space.TIME,
          outputSpace ?? space ?? Space.TIME,
        )
      } else if (inputType || outputType) {
        node = new module.Identity(
          inputType ?? type ?? Type.RATIO,
          outputType ?? type ?? Type.RATIO,
          space ?? Space.TIME,
        )
      } else {
        node = new module.Identity(type ?? Type.RATIO, space ?? Space.TIME)
      }
      break
    }
    case 'InverseFFT':
      node = new module.InverseFFT()
      break
    case 'Logarithm': {
      const { space } = options as NodeOptions<'Logarithm'>
      node = new module.Logarithm(space ?? Space.TIME)
      break
    }
    case 'Modulo': {
      const { type, space } = options as NodeOptions<'Modulo'>
      node = new module.Modulo(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Multiplication': {
      const { type, space } = options as NodeOptions<'Multiplication'>
      node = new module.Multiplication(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Negative': {
      const { type, space } = options as NodeOptions<'Negative'>
      node = new module.Negative(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'NoteToFrequency': {
      const { space } = options as NodeOptions<'NoteToFrequency'>
      node = new module.NoteToFrequency(space ?? Space.TIME)
      break
    }
    case 'NotGate': {
      const { space } = options as NodeOptions<'NotGate'>
      node = new module.NotGate(space ?? Space.TIME)
      break
    }
    case 'Power': {
      const { space } = options as NodeOptions<'Power'>
      node = new module.Power(space ?? Space.TIME)
      break
    }
    case 'Reciprocal': {
      const { type, space } = options as NodeOptions<'Reciprocal'>
      node = new module.Reciprocal(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Trigonometric': {
      const { space } = options as NodeOptions<'Trigonometric'>
      node = new module.Trigonometric(space ?? Space.TIME)
      break
    }

    // Trigger Nodes
    case 'ClockTrigger':
      node = new module.ClockTrigger()
      break
    case 'Differentiator': {
      const { type } = options as NodeOptions<'Differentiator'>
      node = new module.Differentiator(type ?? Type.RATIO)
      break
    }
    case 'Integrator': {
      const { type } = options as NodeOptions<'Integrator'>
      node = new module.Integrator(type ?? Type.RATIO)
      break
    }
    case 'OnOff':
      node = new module.OnOff()
      break
    case 'ResetTrigger':
      node = new module.ResetTrigger()
      break
    case 'SampleAndHold': {
      const { type } = options as NodeOptions<'SampleAndHold'>
      node = new module.SampleAndHold(type ?? Type.RATIO)
      break
    }
    case 'Sequencer': {
      const { type, space } = options as NodeOptions<'Sequencer'>
      node = new module.Sequencer(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'TriggerHold':
      node = new module.TriggerHold()
      break

    // Variable Nodes
    case 'BufferDuration':
      node = new module.BufferDuration()
      break
    case 'BufferRate':
      node = new module.BufferRate()
      break
    case 'SampleDuration':
      node = new module.SampleDuration()
      break
    case 'SampleRate':
      node = new module.SampleRate()
      break

    default:
      throw new Error(`Unsupported node type: ${nodeType}`)
  }
  const { numChannels, numInputChannels, numOutputChannels } = options
  node.setNumInputChannels(numInputChannels ?? numChannels)
  node.setNumOutputChannels(numOutputChannels ?? numChannels)
  return node
}
