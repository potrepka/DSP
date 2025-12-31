import { nanoid } from 'nanoid/non-secure'
import { ID_LENGTH } from '../../constants/proxy'
import type {
  NodeType,
  Options,
  ProxyContext,
  ResponseMessage,
  Target,
} from '../../types'
import { BufferProxy, NodeProcessorProxy, NodeProxy } from './core'
import {
  BiquadProxy,
  MultiplicationProxy,
  PhasorProxy,
  TableOscillatorProxy,
} from './nodes'

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
  private isReady = false

  constructor(workletNode: AudioWorkletNode) {
    this.#context = { sendMessage: this.sendMessage }
    this.#port = workletNode.port
    this.#nodeProcessor = new NodeProcessorProxy(this.#context, {
      __type: 'Target',
      id: 'NodeProcessor',
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
          this.isReady = true
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

  ready = async () => {
    if (this.isReady) {
      return
    }
    return this.readyPromise
  }

  getNodeProcessor = (): NodeProcessorProxy => this.#nodeProcessor

  createBuffer = async (
    options: Options<'Buffer'> = {} as Options<'Buffer'>,
  ): Promise<BufferProxy> => {
    const target = await this.sendMessage<Target>({
      message: 'createObject',
      requestId: this.generateRequestId(),
      objectType: 'Buffer',
      options,
    })
    return new BufferProxy(this.#context, target)
  }

  createNode = async <T extends NodeType>(
    nodeType?: T,
    options?: Options<T>,
  ): Promise<NodeProxy> => {
    const target = await this.sendMessage<Target>({
      message: 'createObject',
      requestId: this.generateRequestId(),
      objectType: nodeType,
      options,
    })
    return new NodeProxy(this.#context, target)
  }

  createBiquad = async (options?: Options<'Biquad'>): Promise<BiquadProxy> =>
    this.createNode('Biquad', options) as Promise<BiquadProxy>

  createPhasor = async (options?: Options<'Phasor'>): Promise<PhasorProxy> =>
    this.createNode('Phasor', options) as Promise<PhasorProxy>

  createTableOscillator = async (
    options?: Options<'TableOscillator'>,
  ): Promise<TableOscillatorProxy> =>
    this.createNode('TableOscillator', options) as Promise<TableOscillatorProxy>

  createMultiplication = async (
    options?: Options<'Multiplication'>,
  ): Promise<MultiplicationProxy> =>
    this.createNode('Multiplication', options) as Promise<MultiplicationProxy>

  delete = (): Promise<void> => {
    return this.sendMessage<void>({
      message: 'delete',
      requestId: this.generateRequestId(),
    })
  }
}
