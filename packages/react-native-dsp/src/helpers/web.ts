export const initializeWebAudio = async (
  moduleUrl: string,
): Promise<{
  addModule: (context: BaseAudioContext) => Promise<void>
  createAudioWorkletNode: (
    context: BaseAudioContext,
    numChannels: number,
  ) => AudioWorkletNode
}> => {
  const addModule = async (context: BaseAudioContext) => {
    await context.audioWorklet.addModule(moduleUrl)
  }

  const createAudioWorkletNode = (
    context: BaseAudioContext,
    numChannels: number,
  ) =>
    new AudioWorkletNode(context, 'dsp', {
      numberOfInputs: 1,
      numberOfOutputs: 1,
      outputChannelCount: [numChannels],
    })

  return {
    addModule,
    createAudioWorkletNode,
  }
}
