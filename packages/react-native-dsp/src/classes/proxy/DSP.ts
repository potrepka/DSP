import { Space, Type } from '../../enums'
import type {
  ObjectType,
  Options,
  OutgoingMessage,
  ProxyContext,
  Target,
} from '../../types'
import { BufferProxy, NodeProcessorProxy } from './core'
import {
  BiquadProxy,
  MultiplicationProxy,
  PhasorProxy,
  TableOscillatorProxy,
} from './nodes'

type PendingRequest = {
  resolve: (value: unknown) => void
  reject: (error: Error) => void
}

export class DSP {
  private readonly context: ProxyContext
  private idCounter = 0
  private requestIdCounter = 0
  private isReady = false
  private readyResolve?: () => void
  private readyPromise: Promise<void>
  private pendingRequests = new Map<string, PendingRequest>()
  public readonly nodeProcessor: NodeProcessorProxy

  constructor(workletNode: AudioWorkletNode) {
    this.context = { port: workletNode.port, call: this.call }
    this.nodeProcessor = new NodeProcessorProxy(this.context)
    this.readyPromise = new Promise((resolve) => {
      this.readyResolve = resolve
    })
    workletNode.port.onmessage = this.handleMessage
  }

  private call = <T>(
    target: Target,
    methodName: string,
    args: unknown[],
  ): Promise<T> => {
    return new Promise((resolve, reject) => {
      const requestId = `req_${this.requestIdCounter++}`
      this.pendingRequests.set(requestId, {
        resolve: resolve as (value: unknown) => void,
        reject,
      })
      this.context.port.postMessage({
        message: 'callMethod',
        requestId,
        target,
        methodName,
        args,
      })
    })
  }

  private handleMessage = (event: MessageEvent<OutgoingMessage>) => {
    const { data } = event
    switch (data.message) {
      case 'setState':
        if (data.state === 'running') {
          this.isReady = true
          this.readyResolve?.()
        }
        break
      case 'response': {
        const pending = this.pendingRequests.get(data.requestId)
        if (pending) {
          this.pendingRequests.delete(data.requestId)
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

  private generateObjectId = () => `obj_${this.idCounter++}`

  private postMessage = (message: unknown) => {
    this.context.port.postMessage(message)
  }

  private createObject = <T extends ObjectType>(
    objectType: T,
    options: Options<T> = {} as Options<T>,
  ) => {
    const objectId = this.generateObjectId()
    this.postMessage({
      message: 'createObject',
      objectId,
      objectType,
      options,
    })
    return objectId
  }

  createBuffer = (options: Options<'Buffer'> = {} as Options<'Buffer'>) => {
    const {
      type = Type.RATIO,
      space = Space.TIME,
      range = 0,
      defaultValue = 0,
      numChannels = 1,
      numSamples = 1,
      data = [],
    } = options
    const objectId = this.createObject('Buffer', {
      type,
      space,
      range,
      defaultValue,
      numChannels,
      numSamples,
      data,
    })
    return new BufferProxy(this.context, objectId)
  }

  createBiquad = (options?: Options<'Biquad'>) => {
    const nodeId = this.createObject('Biquad', options)
    return new BiquadProxy(this.context, nodeId)
  }

  createPhasor = (options?: Options<'Phasor'>) => {
    const nodeId = this.createObject('Phasor', options)
    return new PhasorProxy(this.context, nodeId)
  }

  createTableOscillator = (options?: Options<'TableOscillator'>) => {
    const nodeId = this.createObject('TableOscillator', options)
    return new TableOscillatorProxy(this.context, nodeId)
  }

  createMultiplication = (options?: Options<'Multiplication'>) => {
    const nodeId = this.createObject('Multiplication', options)
    return new MultiplicationProxy(this.context, nodeId)
  }

  delete = () => {
    this.postMessage({ message: 'delete' })
  }
}
