declare class AudioWorkletProcessor {
  readonly port: MessagePort
  process(
    inputs: Float32Array[][],
    outputs: Float32Array[][],
    parameters: Record<string, Float32Array>,
  ): boolean
}

declare const registerProcessor: (
  name: string,
  ctor: new (options: AudioWorkletNodeOptions) => AudioWorkletProcessor,
) => void

declare const currentFrame: number
declare const currentTime: number
declare const sampleRate: number
