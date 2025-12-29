import { Space, Type } from '../enums'
import { ReservedKeyword } from '../enums/module'
import { Node, NodeProps, NodeType, WebAudioModule } from '../types'

export const getReservedKeywords = () =>
  Object.values(ReservedKeyword) as string[]

export const constructNode = <T extends NodeType>(
  module: WebAudioModule,
  nodeType: T,
  props: NodeProps<T>,
) => {
  let node: Node
  switch (nodeType) {
    // Analyzer Nodes
    case 'Recorder': {
      const { type, space, defaultValue } = props as NodeProps<'Recorder'>
      node = new module.Recorder(
        type ?? Type.RATIO,
        space ?? Space.TIME,
        defaultValue ?? 0,
      )
      break
    }

    // Channel Nodes
    case 'ChannelMerger': {
      const { type, space } = props as NodeProps<'ChannelMerger'>
      node = new module.ChannelMerger(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'ChannelSplitter': {
      const { type, space } = props as NodeProps<'ChannelSplitter'>
      node = new module.ChannelSplitter(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'MidSide': {
      const { type, space } = props as NodeProps<'MidSide'>
      node = new module.MidSide(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Spread': {
      const { type, space } = props as NodeProps<'Spread'>
      node = new module.Spread(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'StereoPanner': {
      const { type, space } = props as NodeProps<'StereoPanner'>
      node = new module.StereoPanner(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }

    // Delay Nodes
    case 'Convolver':
      node = new module.Convolver()
      break
    case 'VariableDelay': {
      const { type } = props as NodeProps<'VariableDelay'>
      node = new module.VariableDelay(type ?? Type.RATIO)
      break
    }

    // Dynamics Nodes
    case 'Clipper': {
      const { type, space } = props as NodeProps<'Clipper'>
      node = new module.Clipper(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'CompressorGate':
      node = new module.CompressorGate()
      break
    case 'DryWet': {
      const { type, space } = props as NodeProps<'DryWet'>
      node = new module.DryWet(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Envelope':
      node = new module.Envelope()
      break
    case 'Lag': {
      const { type } = props as NodeProps<'Lag'>
      node = new module.Lag(type ?? Type.RATIO)
      break
    }
    case 'Shaper': {
      const { space } = props as NodeProps<'Shaper'>
      node = new module.Shaper(space ?? Space.TIME)
      break
    }

    // External Nodes
    case 'MidiInput': {
      const { midiBuffer, type } = props as NodeProps<'MidiInput'>
      node = new module.MidiInput(midiBuffer, type ?? Type.RATIO)
      break
    }
    case 'MidiOutput': {
      const { midiBuffer, type } = props as NodeProps<'MidiOutput'>
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
      const { type } = props as NodeProps<'OnePole'>
      node = new module.OnePole(type ?? Type.RATIO)
      break
    }

    // Generator Nodes
    case 'FunctionOscillator': {
      const { type } = props as NodeProps<'FunctionOscillator'>
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
      const { type } = props as NodeProps<'SamplePlayer'>
      node = new module.SamplePlayer(type ?? Type.RATIO)
      break
    }
    case 'TableOscillator': {
      const { type } = props as NodeProps<'TableOscillator'>
      node = new module.TableOscillator(type ?? Type.RATIO)
      break
    }

    // Math Nodes
    case 'AbsoluteValue': {
      const { type, space } = props as NodeProps<'AbsoluteValue'>
      node = new module.AbsoluteValue(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'BooleanMask': {
      const { type, space } = props as NodeProps<'BooleanMask'>
      node = new module.BooleanMask(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Comparison': {
      const { type, space } = props as NodeProps<'Comparison'>
      node = new module.Comparison(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Division': {
      const { type, space } = props as NodeProps<'Division'>
      node = new module.Division(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Floor': {
      const { type, space } = props as NodeProps<'Floor'>
      node = new module.Floor(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'ForwardFFT':
      node = new module.ForwardFFT()
      break
    case 'FrequencyToNote': {
      const { space } = props as NodeProps<'FrequencyToNote'>
      node = new module.FrequencyToNote(space ?? Space.TIME)
      break
    }
    case 'Hyperbolic': {
      const { space } = props as NodeProps<'Hyperbolic'>
      node = new module.Hyperbolic(space ?? Space.TIME)
      break
    }
    case 'Identity': {
      const { type, space, inputType, outputType, inputSpace, outputSpace } =
        props as NodeProps<'Identity'>
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
      const { space } = props as NodeProps<'Logarithm'>
      node = new module.Logarithm(space ?? Space.TIME)
      break
    }
    case 'Modulo': {
      const { type, space } = props as NodeProps<'Modulo'>
      node = new module.Modulo(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Multiplication': {
      const { type, space } = props as NodeProps<'Multiplication'>
      node = new module.Multiplication(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Negative': {
      const { type, space } = props as NodeProps<'Negative'>
      node = new module.Negative(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'NoteToFrequency': {
      const { space } = props as NodeProps<'NoteToFrequency'>
      node = new module.NoteToFrequency(space ?? Space.TIME)
      break
    }
    case 'NotGate': {
      const { space } = props as NodeProps<'NotGate'>
      node = new module.NotGate(space ?? Space.TIME)
      break
    }
    case 'Power': {
      const { space } = props as NodeProps<'Power'>
      node = new module.Power(space ?? Space.TIME)
      break
    }
    case 'Reciprocal': {
      const { type, space } = props as NodeProps<'Reciprocal'>
      node = new module.Reciprocal(type ?? Type.RATIO, space ?? Space.TIME)
      break
    }
    case 'Trigonometric': {
      const { space } = props as NodeProps<'Trigonometric'>
      node = new module.Trigonometric(space ?? Space.TIME)
      break
    }

    // Trigger Nodes
    case 'ClockTrigger':
      node = new module.ClockTrigger()
      break
    case 'Differentiator': {
      const { type } = props as NodeProps<'Differentiator'>
      node = new module.Differentiator(type ?? Type.RATIO)
      break
    }
    case 'Integrator': {
      const { type } = props as NodeProps<'Integrator'>
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
      const { type } = props as NodeProps<'SampleAndHold'>
      node = new module.SampleAndHold(type ?? Type.RATIO)
      break
    }
    case 'Sequencer': {
      const { type, space } = props as NodeProps<'Sequencer'>
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
  return node
}
