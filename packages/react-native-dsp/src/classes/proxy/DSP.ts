import { nanoid } from 'nanoid/non-secure'
import { ID_LENGTH, NODE_PROCESSOR_ID } from '../../constants/proxy'
import {
  ObjectType,
  Options,
  ResponseMessage,
  Target,
} from '../../types/module'
import { ProxyContext } from '../../types/proxy'
import { BufferProxy } from './core/BufferProxy'
import { NodeProcessorProxy } from './core/NodeProcessorProxy'
import { MidiBufferProxy } from './midi/MidiBufferProxy'
import { MidiMessageProxy } from './midi/MidiMessageProxy'
import { AbsoluteValueProxy } from './nodes/AbsoluteValueProxy'
import { BiquadProxy } from './nodes/BiquadProxy'
import { BooleanMaskProxy } from './nodes/BooleanMaskProxy'
import { BufferDurationProxy } from './nodes/BufferDurationProxy'
import { BufferRateProxy } from './nodes/BufferRateProxy'
import { ChannelMergerProxy } from './nodes/ChannelMergerProxy'
import { ChannelSplitterProxy } from './nodes/ChannelSplitterProxy'
import { ClipperProxy } from './nodes/ClipperProxy'
import { ClockTriggerProxy } from './nodes/ClockTriggerProxy'
import { ComparisonProxy } from './nodes/ComparisonProxy'
import { CompressorGateProxy } from './nodes/CompressorGateProxy'
import { ConvolverProxy } from './nodes/ConvolverProxy'
import { CrossoverProxy } from './nodes/CrossoverProxy'
import { DifferentiatorProxy } from './nodes/DifferentiatorProxy'
import { DivisionProxy } from './nodes/DivisionProxy'
import { DryWetProxy } from './nodes/DryWetProxy'
import { EnvelopeProxy } from './nodes/EnvelopeProxy'
import { FloorProxy } from './nodes/FloorProxy'
import { ForwardFFTProxy } from './nodes/ForwardFFTProxy'
import { FrequencyToNoteProxy } from './nodes/FrequencyToNoteProxy'
import { FunctionOscillatorProxy } from './nodes/FunctionOscillatorProxy'
import { HyperbolicProxy } from './nodes/HyperbolicProxy'
import { IdentityProxy } from './nodes/IdentityProxy'
import { IntegratorProxy } from './nodes/IntegratorProxy'
import { InverseFFTProxy } from './nodes/InverseFFTProxy'
import { LagProxy } from './nodes/LagProxy'
import { LogarithmProxy } from './nodes/LogarithmProxy'
import { MidiInputProxy } from './nodes/MidiInputProxy'
import { MidiOutputProxy } from './nodes/MidiOutputProxy'
import { MidSideProxy } from './nodes/MidSideProxy'
import { ModuloProxy } from './nodes/ModuloProxy'
import { MultiplicationProxy } from './nodes/MultiplicationProxy'
import { NegativeProxy } from './nodes/NegativeProxy'
import { NoiseProxy } from './nodes/NoiseProxy'
import { NoteToFrequencyProxy } from './nodes/NoteToFrequencyProxy'
import { NotGateProxy } from './nodes/NotGateProxy'
import { OnePoleProxy } from './nodes/OnePoleProxy'
import { OnOffProxy } from './nodes/OnOffProxy'
import { PhasorProxy } from './nodes/PhasorProxy'
import { PowerProxy } from './nodes/PowerProxy'
import { ReciprocalProxy } from './nodes/ReciprocalProxy'
import { RecorderProxy } from './nodes/RecorderProxy'
import { ResetTriggerProxy } from './nodes/ResetTriggerProxy'
import { SampleAndHoldProxy } from './nodes/SampleAndHoldProxy'
import { SampleDurationProxy } from './nodes/SampleDurationProxy'
import { SamplePlayerProxy } from './nodes/SamplePlayerProxy'
import { SampleRateProxy } from './nodes/SampleRateProxy'
import { SequencerProxy } from './nodes/SequencerProxy'
import { ShaperProxy } from './nodes/ShaperProxy'
import { SpreadProxy } from './nodes/SpreadProxy'
import { StereoPannerProxy } from './nodes/StereoPannerProxy'
import { TableOscillatorProxy } from './nodes/TableOscillatorProxy'
import { TriggerHoldProxy } from './nodes/TriggerHoldProxy'
import { TrigonometricProxy } from './nodes/TrigonometricProxy'
import { VariableDelayProxy } from './nodes/VariableDelayProxy'

type PendingRequest<T = unknown> = {
  resolve: (value: T | PromiseLike<T>) => void
  reject: (error: Error) => void
}

export class DSP {
  readonly #context: ProxyContext
  readonly #port: MessagePort
  readonly #nodeProcessor: NodeProcessorProxy
  readonly #pendingRequests = new Map<string, PendingRequest>()
  private readyResolve?: () => void
  private readyPromise: Promise<void>
  private ready = false
  constructor(workletNode: AudioWorkletNode) {
    this.#context = {
      generateRequestId: this.generateRequestId,
      sendMessage: this.sendMessage,
    }
    this.#port = workletNode.port
    this.#nodeProcessor = new NodeProcessorProxy(this.#context, {
      __type: 'Target',
      id: NODE_PROCESSOR_ID,
    })
    this.readyPromise = new Promise((resolve) => {
      this.readyResolve = resolve
    })
    this.#port.onmessage = this.handleMessage
  }
  private generateRequestId = () => nanoid(ID_LENGTH)
  private sendMessage = <T>(message: {
    message: string
    requestId: string
    [key: string]: unknown
  }): Promise<T> => {
    return new Promise((resolve, reject) => {
      const requestId = this.generateRequestId()
      const messageWithId = { ...message, requestId }
      this.#pendingRequests.set(requestId, {
        resolve: resolve as (value: unknown) => void,
        reject,
      })
      this.#port.postMessage(messageWithId)
    })
  }
  private handleMessage = (event: MessageEvent<ResponseMessage>) => {
    const { data } = event
    switch (data.message) {
      case 'state':
        if (data.state === 'running') {
          this.ready = true
          this.readyResolve?.()
        }
        break
      case 'response': {
        const pending = this.#pendingRequests.get(data.requestId)
        if (pending) {
          this.#pendingRequests.delete(data.requestId)
          if (data.error) {
            pending.reject(new Error(data.error))
          } else {
            pending.resolve(data.result)
          }
        }
        break
      }
    }
  }
  private createObject = <T extends ObjectType>(
    objectType: T,
    options: Options<T> = {} as Options<T>,
  ): Promise<Target> => {
    return this.sendMessage<Target>({
      message: 'createObject',
      requestId: this.generateRequestId(),
      objectType,
      options,
    })
  }
  private callStatic = (
    objectType: string,
    methodName: string,
    args: unknown[],
  ): Promise<Target> => {
    return this.sendMessage<Target>({
      message: 'callStatic',
      requestId: this.generateRequestId(),
      objectType,
      methodName,
      args,
    })
  }
  isReady = async () => {
    if (this.ready) {
      return
    }
    return this.readyPromise
  }
  getNodeProcessor = (): NodeProcessorProxy => this.#nodeProcessor
  delete = (): Promise<void> => {
    return this.sendMessage<void>({
      message: 'delete',
      requestId: this.generateRequestId(),
    })
  }
  createBuffer = async (
    options: Options<'Buffer'> = {} as Options<'Buffer'>,
  ): Promise<BufferProxy> => {
    const target = await this.createObject('Buffer', options)
    return new BufferProxy(this.#context, target)
  }
  createMidiBuffer = async (
    options: Options<'MidiBuffer'> = {} as Options<'MidiBuffer'>,
  ): Promise<MidiBufferProxy> => {
    const target = await this.createObject('MidiBuffer', options)
    return new MidiBufferProxy(this.#context, target)
  }
  createMidiMessage = async (bytes: number[]): Promise<MidiMessageProxy> => {
    if (bytes.length === 0) {
      throw new Error('MidiMessage requires at least 1 byte')
    }
    const target = await (bytes.length <= 3
      ? this.createObject('MidiMessage', {
          byte0: bytes[0],
          byte1: bytes[1],
          byte2: bytes[2],
        })
      : this.callStatic('MidiMessage', 'fromArray', [bytes]))
    return new MidiMessageProxy(this.#context, target)
  }
  createNoteOff = async (
    channel: number,
    noteNumber: number,
    velocity: number,
  ): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'noteOff', [
      channel,
      noteNumber,
      velocity,
    ])
    return new MidiMessageProxy(this.#context, target)
  }
  createNoteOn = async (
    channel: number,
    noteNumber: number,
    velocity: number,
  ): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'noteOn', [
      channel,
      noteNumber,
      velocity,
    ])
    return new MidiMessageProxy(this.#context, target)
  }
  createAftertouch = async (
    channel: number,
    noteNumber: number,
    aftertouchValue: number,
  ): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'aftertouch', [
      channel,
      noteNumber,
      aftertouchValue,
    ])
    return new MidiMessageProxy(this.#context, target)
  }
  createController = async (
    channel: number,
    controllerNumber: number,
    controllerValue: number,
  ): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'controller', [
      channel,
      controllerNumber,
      controllerValue,
    ])
    return new MidiMessageProxy(this.#context, target)
  }
  createProgramChange = async (
    channel: number,
    programNumber: number,
  ): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'programChange', [
      channel,
      programNumber,
    ])
    return new MidiMessageProxy(this.#context, target)
  }
  createChannelPressure = async (
    channel: number,
    channelPressureValue: number,
  ): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'channelPressure', [
      channel,
      channelPressureValue,
    ])
    return new MidiMessageProxy(this.#context, target)
  }
  createPitchWheel = async (
    channel: number,
    pitchWheelValue: number,
  ): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'pitchWheel', [
      channel,
      pitchWheelValue,
    ])
    return new MidiMessageProxy(this.#context, target)
  }
  createAllNotesOff = async (channel: number): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'allNotesOff', [
      channel,
    ])
    return new MidiMessageProxy(this.#context, target)
  }
  createSongPositionPointer = async (
    songPositionPointerMidiBeat: number,
  ): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'songPositionPointer', [
      songPositionPointerMidiBeat,
    ])
    return new MidiMessageProxy(this.#context, target)
  }
  createMidiClock = async (): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'midiClock', [])
    return new MidiMessageProxy(this.#context, target)
  }
  createMidiStart = async (): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'midiStart', [])
    return new MidiMessageProxy(this.#context, target)
  }
  createMidiContinue = async (): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'midiContinue', [])
    return new MidiMessageProxy(this.#context, target)
  }
  createMidiStop = async (): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'midiStop', [])
    return new MidiMessageProxy(this.#context, target)
  }
  createRecorder = async (
    options?: Options<'Recorder'>,
  ): Promise<RecorderProxy> => {
    const target = await this.createObject('Recorder', options)
    return new RecorderProxy(this.#context, target)
  }
  createChannelMerger = async (
    options?: Options<'ChannelMerger'>,
  ): Promise<ChannelMergerProxy> => {
    const target = await this.createObject('ChannelMerger', options)
    return new ChannelMergerProxy(this.#context, target)
  }
  createChannelSplitter = async (
    options?: Options<'ChannelSplitter'>,
  ): Promise<ChannelSplitterProxy> => {
    const target = await this.createObject('ChannelSplitter', options)
    return new ChannelSplitterProxy(this.#context, target)
  }
  createMidSide = async (
    options?: Options<'MidSide'>,
  ): Promise<MidSideProxy> => {
    const target = await this.createObject('MidSide', options)
    return new MidSideProxy(this.#context, target)
  }
  createSpread = async (options?: Options<'Spread'>): Promise<SpreadProxy> => {
    const target = await this.createObject('Spread', options)
    return new SpreadProxy(this.#context, target)
  }
  createStereoPanner = async (
    options?: Options<'StereoPanner'>,
  ): Promise<StereoPannerProxy> => {
    const target = await this.createObject('StereoPanner', options)
    return new StereoPannerProxy(this.#context, target)
  }
  createConvolver = async (
    options?: Options<'Convolver'>,
  ): Promise<ConvolverProxy> => {
    const target = await this.createObject('Convolver', options)
    return new ConvolverProxy(this.#context, target)
  }
  createVariableDelay = async (
    options?: Options<'VariableDelay'>,
  ): Promise<VariableDelayProxy> => {
    const target = await this.createObject('VariableDelay', options)
    return new VariableDelayProxy(this.#context, target)
  }
  createClipper = async (
    options?: Options<'Clipper'>,
  ): Promise<ClipperProxy> => {
    const target = await this.createObject('Clipper', options)
    return new ClipperProxy(this.#context, target)
  }
  createCompressorGate = async (
    options?: Options<'CompressorGate'>,
  ): Promise<CompressorGateProxy> => {
    const target = await this.createObject('CompressorGate', options)
    return new CompressorGateProxy(this.#context, target)
  }
  createDryWet = async (options?: Options<'DryWet'>): Promise<DryWetProxy> => {
    const target = await this.createObject('DryWet', options)
    return new DryWetProxy(this.#context, target)
  }
  createEnvelope = async (
    options?: Options<'Envelope'>,
  ): Promise<EnvelopeProxy> => {
    const target = await this.createObject('Envelope', options)
    return new EnvelopeProxy(this.#context, target)
  }
  createLag = async (options?: Options<'Lag'>): Promise<LagProxy> => {
    const target = await this.createObject('Lag', options)
    return new LagProxy(this.#context, target)
  }
  createShaper = async (options?: Options<'Shaper'>): Promise<ShaperProxy> => {
    const target = await this.createObject('Shaper', options)
    return new ShaperProxy(this.#context, target)
  }
  createMidiInput = async (
    options?: Options<'MidiInput'>,
  ): Promise<MidiInputProxy> => {
    const target = await this.createObject('MidiInput', options)
    return new MidiInputProxy(this.#context, target)
  }
  createMidiOutput = async (
    options?: Options<'MidiOutput'>,
  ): Promise<MidiOutputProxy> => {
    const target = await this.createObject('MidiOutput', options)
    return new MidiOutputProxy(this.#context, target)
  }
  createBiquad = async (options?: Options<'Biquad'>): Promise<BiquadProxy> => {
    const target = await this.createObject('Biquad', options)
    return new BiquadProxy(this.#context, target)
  }
  createCrossover = async (
    options?: Options<'Crossover'>,
  ): Promise<CrossoverProxy> => {
    const target = await this.createObject('Crossover', options)
    return new CrossoverProxy(this.#context, target)
  }
  createOnePole = async (
    options?: Options<'OnePole'>,
  ): Promise<OnePoleProxy> => {
    const target = await this.createObject('OnePole', options)
    return new OnePoleProxy(this.#context, target)
  }
  createFunctionOscillator = async (
    options?: Options<'FunctionOscillator'>,
  ): Promise<FunctionOscillatorProxy> => {
    const target = await this.createObject('FunctionOscillator', options)
    return new FunctionOscillatorProxy(this.#context, target)
  }
  createNoise = async (options?: Options<'Noise'>): Promise<NoiseProxy> => {
    const target = await this.createObject('Noise', options)
    return new NoiseProxy(this.#context, target)
  }
  createPhasor = async (options?: Options<'Phasor'>): Promise<PhasorProxy> => {
    const target = await this.createObject('Phasor', options)
    return new PhasorProxy(this.#context, target)
  }
  createSamplePlayer = async (
    options?: Options<'SamplePlayer'>,
  ): Promise<SamplePlayerProxy> => {
    const target = await this.createObject('SamplePlayer', options)
    return new SamplePlayerProxy(this.#context, target)
  }
  createTableOscillator = async (
    options?: Options<'TableOscillator'>,
  ): Promise<TableOscillatorProxy> => {
    const target = await this.createObject('TableOscillator', options)
    return new TableOscillatorProxy(this.#context, target)
  }
  createAbsoluteValue = async (
    options?: Options<'AbsoluteValue'>,
  ): Promise<AbsoluteValueProxy> => {
    const target = await this.createObject('AbsoluteValue', options)
    return new AbsoluteValueProxy(this.#context, target)
  }
  createBooleanMask = async (
    options?: Options<'BooleanMask'>,
  ): Promise<BooleanMaskProxy> => {
    const target = await this.createObject('BooleanMask', options)
    return new BooleanMaskProxy(this.#context, target)
  }
  createComparison = async (
    options?: Options<'Comparison'>,
  ): Promise<ComparisonProxy> => {
    const target = await this.createObject('Comparison', options)
    return new ComparisonProxy(this.#context, target)
  }
  createDivision = async (
    options?: Options<'Division'>,
  ): Promise<DivisionProxy> => {
    const target = await this.createObject('Division', options)
    return new DivisionProxy(this.#context, target)
  }
  createFloor = async (options?: Options<'Floor'>): Promise<FloorProxy> => {
    const target = await this.createObject('Floor', options)
    return new FloorProxy(this.#context, target)
  }
  createForwardFFT = async (
    options?: Options<'ForwardFFT'>,
  ): Promise<ForwardFFTProxy> => {
    const target = await this.createObject('ForwardFFT', options)
    return new ForwardFFTProxy(this.#context, target)
  }
  createFrequencyToNote = async (
    options?: Options<'FrequencyToNote'>,
  ): Promise<FrequencyToNoteProxy> => {
    const target = await this.createObject('FrequencyToNote', options)
    return new FrequencyToNoteProxy(this.#context, target)
  }
  createHyperbolic = async (
    options?: Options<'Hyperbolic'>,
  ): Promise<HyperbolicProxy> => {
    const target = await this.createObject('Hyperbolic', options)
    return new HyperbolicProxy(this.#context, target)
  }
  createIdentity = async (
    options?: Options<'Identity'>,
  ): Promise<IdentityProxy> => {
    const target = await this.createObject('Identity', options)
    return new IdentityProxy(this.#context, target)
  }
  createInverseFFT = async (
    options?: Options<'InverseFFT'>,
  ): Promise<InverseFFTProxy> => {
    const target = await this.createObject('InverseFFT', options)
    return new InverseFFTProxy(this.#context, target)
  }
  createLogarithm = async (
    options?: Options<'Logarithm'>,
  ): Promise<LogarithmProxy> => {
    const target = await this.createObject('Logarithm', options)
    return new LogarithmProxy(this.#context, target)
  }
  createModulo = async (options?: Options<'Modulo'>): Promise<ModuloProxy> => {
    const target = await this.createObject('Modulo', options)
    return new ModuloProxy(this.#context, target)
  }
  createMultiplication = async (
    options?: Options<'Multiplication'>,
  ): Promise<MultiplicationProxy> => {
    const target = await this.createObject('Multiplication', options)
    return new MultiplicationProxy(this.#context, target)
  }
  createNegative = async (
    options?: Options<'Negative'>,
  ): Promise<NegativeProxy> => {
    const target = await this.createObject('Negative', options)
    return new NegativeProxy(this.#context, target)
  }
  createNoteToFrequency = async (
    options?: Options<'NoteToFrequency'>,
  ): Promise<NoteToFrequencyProxy> => {
    const target = await this.createObject('NoteToFrequency', options)
    return new NoteToFrequencyProxy(this.#context, target)
  }
  createNotGate = async (
    options?: Options<'NotGate'>,
  ): Promise<NotGateProxy> => {
    const target = await this.createObject('NotGate', options)
    return new NotGateProxy(this.#context, target)
  }
  createPower = async (options?: Options<'Power'>): Promise<PowerProxy> => {
    const target = await this.createObject('Power', options)
    return new PowerProxy(this.#context, target)
  }
  createReciprocal = async (
    options?: Options<'Reciprocal'>,
  ): Promise<ReciprocalProxy> => {
    const target = await this.createObject('Reciprocal', options)
    return new ReciprocalProxy(this.#context, target)
  }
  createTrigonometric = async (
    options?: Options<'Trigonometric'>,
  ): Promise<TrigonometricProxy> => {
    const target = await this.createObject('Trigonometric', options)
    return new TrigonometricProxy(this.#context, target)
  }
  createClockTrigger = async (
    options?: Options<'ClockTrigger'>,
  ): Promise<ClockTriggerProxy> => {
    const target = await this.createObject('ClockTrigger', options)
    return new ClockTriggerProxy(this.#context, target)
  }
  createDifferentiator = async (
    options?: Options<'Differentiator'>,
  ): Promise<DifferentiatorProxy> => {
    const target = await this.createObject('Differentiator', options)
    return new DifferentiatorProxy(this.#context, target)
  }
  createIntegrator = async (
    options?: Options<'Integrator'>,
  ): Promise<IntegratorProxy> => {
    const target = await this.createObject('Integrator', options)
    return new IntegratorProxy(this.#context, target)
  }
  createOnOff = async (options?: Options<'OnOff'>): Promise<OnOffProxy> => {
    const target = await this.createObject('OnOff', options)
    return new OnOffProxy(this.#context, target)
  }
  createResetTrigger = async (
    options?: Options<'ResetTrigger'>,
  ): Promise<ResetTriggerProxy> => {
    const target = await this.createObject('ResetTrigger', options)
    return new ResetTriggerProxy(this.#context, target)
  }
  createSampleAndHold = async (
    options?: Options<'SampleAndHold'>,
  ): Promise<SampleAndHoldProxy> => {
    const target = await this.createObject('SampleAndHold', options)
    return new SampleAndHoldProxy(this.#context, target)
  }
  createSequencer = async (
    options?: Options<'Sequencer'>,
  ): Promise<SequencerProxy> => {
    const target = await this.createObject('Sequencer', options)
    return new SequencerProxy(this.#context, target)
  }
  createTriggerHold = async (
    options?: Options<'TriggerHold'>,
  ): Promise<TriggerHoldProxy> => {
    const target = await this.createObject('TriggerHold', options)
    return new TriggerHoldProxy(this.#context, target)
  }
  createBufferDuration = async (
    options?: Options<'BufferDuration'>,
  ): Promise<BufferDurationProxy> => {
    const target = await this.createObject('BufferDuration', options)
    return new BufferDurationProxy(this.#context, target)
  }
  createBufferRate = async (
    options?: Options<'BufferRate'>,
  ): Promise<BufferRateProxy> => {
    const target = await this.createObject('BufferRate', options)
    return new BufferRateProxy(this.#context, target)
  }
  createSampleDuration = async (
    options?: Options<'SampleDuration'>,
  ): Promise<SampleDurationProxy> => {
    const target = await this.createObject('SampleDuration', options)
    return new SampleDurationProxy(this.#context, target)
  }
  createSampleRate = async (
    options?: Options<'SampleRate'>,
  ): Promise<SampleRateProxy> => {
    const target = await this.createObject('SampleRate', options)
    return new SampleRateProxy(this.#context, target)
  }
}
