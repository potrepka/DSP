import { NodeProcessorOptions } from '../types/module'

export const initializeWebAudio = async (
  moduleUrl: string,
): Promise<{
  addModule: (context: BaseAudioContext) => Promise<void>
  createAudioWorkletNode: (
    context: BaseAudioContext,
    options: NodeProcessorOptions,
  ) => AudioWorkletNode
}> => {
  const addModule = async (context: BaseAudioContext) => {
    await context.audioWorklet.addModule(moduleUrl)
  }

  const createAudioWorkletNode = (
    context: BaseAudioContext,
    options: NodeProcessorOptions,
  ) =>
    new AudioWorkletNode(context, 'dsp', {
      numberOfInputs: 1,
      numberOfOutputs: 1,
      outputChannelCount: [options.numOutputChannels],
      processorOptions: options,
    })

  return {
    addModule,
    createAudioWorkletNode,
  }
}
