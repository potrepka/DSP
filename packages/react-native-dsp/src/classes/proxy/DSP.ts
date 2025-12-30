import { Space, Type } from '../../enums'
import type {
  BufferOptions,
  NodeOptions,
  NodeType,
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
    const call = <T>(
      target: Target,
      functionName: string,
      args: unknown[],
    ): Promise<T> => {
      return new Promise((resolve, reject) => {
        const requestId = `req_${this.requestIdCounter++}`
        this.pendingRequests.set(requestId, {
          resolve: resolve as (value: unknown) => void,
          reject,
        })
        workletNode.port.postMessage({
          message: 'call',
          requestId,
          target,
          functionName,
          args,
        })
      })
    }
    this.context = {
      port: workletNode.port,
      call,
    }
    this.nodeProcessor = new NodeProcessorProxy(this.context)
    this.readyPromise = new Promise((resolve) => {
      this.readyResolve = resolve
    })
    workletNode.port.onmessage = (event: MessageEvent<OutgoingMessage>) => {
      const { data } = event
      if (data.message === 'setState' && data.state === 'running') {
        this.isReady = true
        this.readyResolve?.()
      } else if (data.message === 'response') {
        const pending = this.pendingRequests.get(data.requestId)
        if (pending) {
          this.pendingRequests.delete(data.requestId)
          if (data.error) {
            pending.reject(new Error(data.error))
          } else {
            pending.resolve(data.result)
          }
        }
      }
    }
  }
  ready = async () => {
    if (this.isReady) {
      return
    }
    return this.readyPromise
  }
  private generateBufferId = () => `buffer_${this.idCounter++}`
  private generateNodeId = () => `node_${this.idCounter++}`
  private postMessage = (message: unknown) => {
    this.context.port.postMessage(message)
  }
  createBuffer = (options: BufferOptions) => {
    const bufferId = this.generateBufferId()
    const {
      type = Type.RATIO,
      space = Space.TIME,
      range = 0,
      defaultValue = 0,
      numChannels,
      numSamples,
      data = [],
    } = options
    this.postMessage({
      message: 'createBuffer',
      bufferId,
      options: {
        type,
        space,
        range,
        defaultValue,
        numChannels,
        numSamples,
        data,
      },
    })
    return new BufferProxy(this.context, bufferId)
  }
  private createNode = <T extends NodeType>(
    nodeType: T,
    options: NodeOptions<T>,
  ) => {
    const nodeId = this.generateNodeId()
    this.postMessage({
      message: 'createNode',
      nodeId,
      nodeType,
      options,
    })
    return nodeId
  }
  createBiquad = (options: NodeOptions<'Biquad'>) => {
    const nodeId = this.createNode('Biquad', options)
    return new BiquadProxy(this.context, nodeId)
  }
  createPhasor = (options: NodeOptions<'Phasor'>) => {
    const nodeId = this.createNode('Phasor', options)
    return new PhasorProxy(this.context, nodeId)
  }
  createTableOscillator = (options: NodeOptions<'TableOscillator'>) => {
    const nodeId = this.createNode('TableOscillator', options)
    return new TableOscillatorProxy(this.context, nodeId)
  }
  createMultiplication = (options: NodeOptions<'Multiplication'>) => {
    const nodeId = this.createNode('Multiplication', options)
    return new MultiplicationProxy(this.context, nodeId)
  }
  delete = () => {
    this.postMessage({ message: 'delete' })
  }
}
