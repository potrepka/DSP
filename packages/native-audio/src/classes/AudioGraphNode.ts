import {
  isWorkletProcessorRegistered,
  registerAudioWorkletProcessor,
} from '../helpers/module'

export type AudioGraphNodeConfig = {
  numberOfInputs?: number
  numberOfOutputs?: number
  outputChannelCount?: number[]
  processorName?: string
  wasmModuleUrl?: string
  processorUrl?: string
}

export type TestResult = {
  leftPeak: number
  rightPeak: number
  arrayLeft: string[]
  arrayRight: string[]
  arrayLength: number
  sampleRate: number
  numChannels: number
  bufferSize: number
}

export class AudioGraphNode extends AudioWorkletNode {
  private initializationPromise: Promise<void>
  private resolveInitialization!: () => void
  private rejectInitialization!: (error: Error) => void
  private _moduleInfo?: { constants: string[]; functions: string[] }
  static create = async (
    audioContext: AudioContext,
    config: AudioGraphNodeConfig = {}
  ): Promise<AudioGraphNode> => {
    if (!isWorkletProcessorRegistered()) {
      await registerAudioWorkletProcessor(
        audioContext,
        config.processorName,
        config.wasmModuleUrl,
        config.processorUrl
      )
    }
    const node = new AudioGraphNode(audioContext, config)
    await node.waitForInitialization()
    return node
  }
  constructor(context: AudioContext, config: AudioGraphNodeConfig = {}) {
    const {
      numberOfInputs = 0,
      numberOfOutputs = 1,
      outputChannelCount = [2],
      processorName = 'audio-graph-processor',
    } = config
    super(context, processorName, {
      numberOfInputs,
      numberOfOutputs,
      outputChannelCount,
    })
    this.initializationPromise = new Promise<void>((resolve, reject) => {
      this.resolveInitialization = resolve
      this.rejectInitialization = reject
    })
    this.port.onmessage = this.handleProcessorMessage.bind(this)
    this.port.postMessage({ type: 'init' })
  }
  waitForInitialization = (): Promise<void> => {
    return this.initializationPromise
  }
  private handleProcessorMessage = (event: MessageEvent): void => {
    const { type, data, error } = event.data
    switch (type) {
      case 'initialized':
        this.resolveInitialization()
        break
      case 'error':
        this.rejectInitialization(new Error(error))
        break
      case 'moduleInfo':
        this._moduleInfo = data
        break
      case 'testComplete':
      case 'engineSetup':
      case 'cleanupComplete':
        break
      default:
        console.warn(`Unknown message type from processor: ${type}`)
    }
  }
  getModuleInfo = async (): Promise<{
    constants: string[]
    functions: string[]
  }> => {
    return new Promise((resolve) => {
      const handler = (event: MessageEvent) => {
        if (event.data.type === 'moduleInfo') {
          this.port.removeEventListener('message', handler)
          resolve(event.data.data)
        }
      }
      this.port.addEventListener('message', handler)
      this.port.postMessage({ type: 'getModule' })
    })
  }
  runTest = (config: {
    sampleRate: number
    numChannels: number
    bufferSize: number
  }): Promise<TestResult> => {
    return new Promise((resolve) => {
      const handler = (event: MessageEvent) => {
        if (event.data.type === 'testComplete') {
          this.port.removeEventListener('message', handler)
          resolve(event.data.data)
        }
      }
      this.port.addEventListener('message', handler)
      this.port.postMessage({ type: 'runTest', data: config })
    })
  }
  setupEngine = (config: {
    sampleRate: number
    numChannels: number
    bufferSize: number
  }): Promise<void> => {
    return new Promise((resolve) => {
      const handler = (event: MessageEvent) => {
        if (event.data.type === 'engineSetup') {
          this.port.removeEventListener('message', handler)
          resolve()
        }
      }
      this.port.addEventListener('message', handler)
      this.port.postMessage({ type: 'setupEngine', data: config })
    })
  }
  cleanup = async (): Promise<void> => {
    return new Promise((resolve) => {
      const handler = (event: MessageEvent) => {
        if (event.data.type === 'cleanupComplete') {
          this.port.removeEventListener('message', handler)
          resolve()
        }
      }
      this.port.addEventListener('message', handler)
      this.port.postMessage({ type: 'cleanup' })
    })
  }
  override disconnect = (): void => {
    this.cleanup().then(() => {
      super.disconnect()
    })
  }
}
