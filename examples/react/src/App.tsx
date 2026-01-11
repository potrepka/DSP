import {
  DSP,
  initializeWebAudio,
  NodeProcessorOptions,
} from '@potrepka/react-native-dsp'
// @ts-ignore
import wasmModuleUrl from '@potrepka/react-native-dsp/web/build/react-native-dsp.js?url'
import { useEffect, useState } from 'react'

export const App = () => {
  const [status, setStatus] = useState({
    message: 'Initializing...',
    type: 'info',
  })
  const [output, setOutput] = useState<string[]>([])
  const [buttonsEnabled, setButtonsEnabled] = useState({
    test: false,
    play: false,
  })
  const [isPlaying, setIsPlaying] = useState(false)
  const [addModule, setAddModule] = useState<
    ((context: BaseAudioContext) => Promise<void>) | undefined
  >(undefined)
  const [createAudioWorkletNode, setCreateAudioWorkletNode] = useState<
    | ((
        context: BaseAudioContext,
        options: NodeProcessorOptions,
      ) => AudioWorkletNode)
    | undefined
  >(undefined)
  useEffect(() => {
    initialize()
  }, [])
  const addLog = (message: string) => {
    setOutput((prev) => [...prev, message])
  }
  const initialize = async () => {
    setOutput([])
    setStatus({ message: 'Loading WASM module...', type: 'info' })
    addLog('Loading WASM module...')
    try {
      const { addModule, createAudioWorkletNode } =
        await initializeWebAudio(wasmModuleUrl)
      setAddModule(() => addModule)
      setCreateAudioWorkletNode(() => createAudioWorkletNode)
      setStatus({
        message: 'Initialization completed',
        type: 'success',
      })
      addLog('\n✅ Initialization completed')
      setButtonsEnabled({ test: true, play: true })
    } catch (error) {
      setStatus({
        message: `Error: ${(error as Error).message}`,
        type: 'error',
      })
      addLog(`\n❌ Error: ${(error as Error).message}`)
      console.error(error)
    }
  }
  const setupTest = async (audioWorkletNode: AudioWorkletNode) => {
    // Create the DSP instance
    const dsp = new DSP(audioWorkletNode)

    // Wait for the module to initialize
    await dsp.isReady()

    // Create nodes
    const phasor = await dsp.createPhasor()
    const osc = await dsp.createTableOscillator()
    const filter = await dsp.createBiquad()
    const gain = await dsp.createMultiplication()

    // Set number of channels
    await phasor.setNumChannels(2)
    await osc.setNumChannels(2)
    await filter.setNumChannels(2)
    await gain.setNumChannels(2)

    // Set input values
    await phasor.getFrequency().setAllChannelValues(55)
    await filter.getFrequency().setAllChannelValues(880)
    await gain.getFactor().setAllChannelValues(0.5)

    // Create wavetable
    const sawtoothBufferSize = 2048
    const sawtooth = await dsp.createBuffer({
      numChannels: 1,
      numSamples: sawtoothBufferSize,
    })
    const sawtoothWrapper = await sawtooth.getWrapper()
    for (let sample = 0; sample < sawtoothBufferSize; sample++) {
      const phase = sample / sawtoothBufferSize
      const value = 2 * ((phase + 0.5) % 1) - 1
      sawtoothWrapper.setSample(0, sample, value)
    }

    // Add wavetable to oscillator
    await osc.getTables().push_back(sawtooth)

    // Connect the graph
    await phasor.getOutput().connect(await osc.getPhase())
    await osc.getOutput().connect(await filter.getInput())
    await filter.getOutput().connect(await gain.getInput())
    await gain
      .getOutput()
      .connect(await dsp.getNodeProcessor().getAudioOutput())

    return dsp
  }
  const testOutput = async () => {
    if (!addModule || !createAudioWorkletNode) {
      return
    }
    setOutput([])
    setStatus({ message: 'Testing output...', type: 'info' })
    try {
      const numInputChannels = 0
      const numOutputChannels = 2
      const numSamples = 48000
      const sampleRate = 48000
      addLog('Creating OfflineAudioContext...')
      const offlineAudioContext = new OfflineAudioContext(
        numOutputChannels,
        numSamples,
        sampleRate,
      )
      addLog(`\nSample Rate: ${offlineAudioContext.sampleRate} Hz`)
      addLog(`Channels: ${offlineAudioContext.destination.channelCount}`)
      addLog(`Length: ${offlineAudioContext.length} samples`)
      addLog('\nPreparing OfflineAudioContext...')
      await addModule(offlineAudioContext)
      const offlineAudioWorkletNode = createAudioWorkletNode(
        offlineAudioContext,
        {
          numInputChannels,
          numOutputChannels,
          numSamples: 128,
          sampleRate,
        },
      )
      addLog('\nRunning test...')
      const dsp = await setupTest(offlineAudioWorkletNode)
      offlineAudioWorkletNode.connect(offlineAudioContext.destination)
      const renderedBuffer = await offlineAudioContext.startRendering()
      offlineAudioWorkletNode.disconnect()
      await dsp.delete()
      addLog('\nOutput analysis:')
      const left = renderedBuffer.getChannelData(0)
      const right = renderedBuffer.getChannelData(1)
      let leftPeak = 0
      let rightPeak = 0
      for (let i = 0; i < numSamples; i++) {
        const leftAmplitude = Math.abs(left[i])
        const rightAmplitude = Math.abs(right[i])
        if (leftAmplitude > leftPeak) {
          leftPeak = leftAmplitude
        }
        if (rightAmplitude > rightPeak) {
          rightPeak = rightAmplitude
        }
      }
      const arrayLength = 8
      const arrayLeft = []
      for (let i = 0; i < arrayLength; i++) {
        arrayLeft.push(left[i])
      }
      const arrayRight = []
      for (let i = 0; i < arrayLength; i++) {
        arrayRight.push(right[i])
      }
      addLog(`Peak amplitude (L): ${leftPeak}`)
      addLog(`Peak amplitude (R): ${rightPeak}`)
      addLog(
        `First ${arrayLength} samples (L): [\n  ${arrayLeft.join(',\n  ')}\n]`,
      )
      addLog(
        `First ${arrayLength} samples (R): [\n  ${arrayRight.join(',\n  ')}\n]`,
      )
      if (leftPeak > 0 || rightPeak > 0) {
        setStatus({ message: 'Test completed', type: 'success' })
        addLog('\n✅ Test completed')
      } else {
        setStatus({
          message: 'Test completed (silence detected)',
          type: 'warning',
        })
        addLog('\n⚠️ Test completed (silence detected)')
      }
    } catch (error) {
      addLog(`\n❌ Error: ${(error as Error).message}`)
      console.error(error)
    }
  }
  const testPlayback = async () => {
    if (!addModule || !createAudioWorkletNode || isPlaying) {
      return
    }
    setOutput([])
    setStatus({ message: 'Testing playback...', type: 'info' })
    setIsPlaying(true)
    setButtonsEnabled({ test: false, play: false })
    try {
      addLog('Creating AudioContext...')
      const audioContext = new AudioContext({ sampleRate: 48000 })
      if (audioContext.state === 'suspended') {
        await audioContext.resume()
      }
      addLog(`\nSample Rate: ${audioContext.sampleRate} Hz`)
      addLog(`Channels: ${audioContext.destination.channelCount}`)
      addLog(
        `Base Latency: ${
          audioContext.baseLatency * audioContext.sampleRate
        } samples (${audioContext.baseLatency} seconds)`,
      )
      addLog(
        `Output Latency: ${
          audioContext.outputLatency * audioContext.sampleRate
        } samples (${audioContext.outputLatency} seconds)`,
      )
      addLog('\nPreparing AudioContext...')
      await addModule(audioContext)
      const audioWorkletNode = createAudioWorkletNode(audioContext, {
        numInputChannels: 0,
        numOutputChannels: audioContext.destination.channelCount,
        numSamples: 128,
        sampleRate: audioContext.sampleRate,
      })
      addLog(`\nRunning test...`)
      const dsp = await setupTest(audioWorkletNode)
      audioWorkletNode.connect(audioContext.destination)
      const numSeconds = 2
      addLog(`\nPlaying for ${numSeconds} seconds...`)
      await new Promise((resolve) => setTimeout(resolve, numSeconds * 1000))
      addLog('\nStopping...')
      audioWorkletNode.disconnect()
      await audioContext.close()
      await dsp.delete()
      setStatus({ message: 'Playback completed', type: 'success' })
      addLog('\n✅ Playback completed')
      setIsPlaying(false)
      setButtonsEnabled({ test: true, play: true })
    } catch (error) {
      addLog(`\n❌ Error: ${(error as Error).message}`)
      console.error(error)
      setIsPlaying(false)
      setButtonsEnabled({ test: true, play: true })
    }
  }
  return (
    <div
      style={{
        fontFamily: 'Arial, sans-serif',
        maxWidth: '800px',
        margin: '50px auto',
        padding: '20px',
      }}
    >
      <h1>DSP React Demo</h1>
      <div
        style={{
          margin: '10px 0',
          padding: '10px',
          borderRadius: '5px',
          background:
            status.type === 'success'
              ? '#d4edda'
              : status.type === 'warning'
                ? '#fff3cd'
                : status.type === 'error'
                  ? '#f8d7da'
                  : '#d1ecf1',
          color:
            status.type === 'success'
              ? '#155724'
              : status.type === 'warning'
                ? '#856404'
                : status.type === 'error'
                  ? '#721c24'
                  : '#0c5460',
        }}
      >
        {status.message}
      </div>
      <div
        style={{
          marginTop: '20px',
          marginBottom: '20px',
          display: 'flex',
          gap: '10px',
        }}
      >
        <button
          onClick={testOutput}
          disabled={!buttonsEnabled.test}
          style={{
            padding: '10px 20px',
            fontSize: '16px',
            cursor: 'pointer',
          }}
        >
          Test Output
        </button>
        <button
          onClick={testPlayback}
          disabled={!buttonsEnabled.play}
          style={{
            padding: '10px 20px',
            fontSize: '16px',
            cursor: 'pointer',
          }}
        >
          Test Playback
        </button>
      </div>
      <div
        style={{
          marginTop: '20px',
          padding: '15px',
          background: '#f5f5f5',
          borderRadius: '5px',
          whiteSpace: 'pre-wrap',
          fontFamily: 'monospace',
        }}
      >
        {output.join('\n')}
      </div>
    </div>
  )
}
