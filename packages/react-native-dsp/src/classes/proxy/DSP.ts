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
import { MidiMessageProxy } from './midi/MidiMessageProxy'
import { BiquadProxy } from './nodes/BiquadProxy'
import { MultiplicationProxy } from './nodes/MultiplicationProxy'
import { PhasorProxy } from './nodes/PhasorProxy'
import { TableOscillatorProxy } from './nodes/TableOscillatorProxy'

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
  createBiquad = async (options?: Options<'Biquad'>): Promise<BiquadProxy> => {
    const target = await this.createObject('Biquad', options)
    return new BiquadProxy(this.#context, target)
  }
  createPhasor = async (options?: Options<'Phasor'>): Promise<PhasorProxy> => {
    const target = await this.createObject('Phasor', options)
    return new PhasorProxy(this.#context, target)
  }
  createTableOscillator = async (
    options?: Options<'TableOscillator'>,
  ): Promise<TableOscillatorProxy> => {
    const target = await this.createObject('TableOscillator', options)
    return new TableOscillatorProxy(this.#context, target)
  }
  createMultiplication = async (
    options?: Options<'Multiplication'>,
  ): Promise<MultiplicationProxy> => {
    const target = await this.createObject('Multiplication', options)
    return new MultiplicationProxy(this.#context, target)
  }
  createMidiMessage = async (bytes: number[]): Promise<MidiMessageProxy> => {
    const target = await this.callStatic('MidiMessage', 'fromArray', [bytes])
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
}
