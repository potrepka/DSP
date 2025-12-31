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
    this.#context = { sendMessage: this.sendMessage }
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

  isReady = async () => {
    if (this.ready) {
      return
    }
    return this.readyPromise
  }

  delete = (): Promise<void> => {
    return this.sendMessage<void>({
      message: 'delete',
      requestId: this.generateRequestId(),
    })
  }

  getNodeProcessor = (): NodeProcessorProxy => this.#nodeProcessor

  createObject = <T extends ObjectType>(
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
}
