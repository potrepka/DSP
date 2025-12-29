import {
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
      const { addModule, createAudioWorkletNode } = await initializeWebAudio(
        wasmModuleUrl,
      )
      setAddModule(() => addModule)
      setCreateAudioWorkletNode(() => createAudioWorkletNode)
      setStatus({
        message: 'Initialization completed',
        type: 'success',
      })
      addLog('\n✅ Initialization completed')
      setButtonsEnabled({ test: true, play: true })
    } catch (error: any) {
      setStatus({
        message: `Error: ${error.message}`,
        type: 'error',
      })
      addLog(`\n❌ Error: ${error.message}`)
      console.error(error)
    }
  }
  const setupTest = (audioWorkletNode: AudioWorkletNode) => {
    audioWorkletNode.port.postMessage({
      message: 'createNode',
      id: 'Phasor',
      nodeType: 'Phasor',
    })
    audioWorkletNode.port.postMessage({
      message: 'createNode',
      id: 'Multiplication',
      nodeType: 'Multiplication',
    })
    audioWorkletNode.port.postMessage({
      message: 'setInputValue',
      nodeId: 'Phasor',
      inputName: 'Frequency',
      value: 55,
    })
    audioWorkletNode.port.postMessage({
      message: 'setInputValue',
      nodeId: 'Multiplication',
      inputName: 'Factor',
      value: 0.5,
    })
    audioWorkletNode.port.postMessage({
      message: 'connect',
      sourceNodeId: 'Phasor',
      sourceOutputName: 'Output',
      destinationNodeId: 'Multiplication',
      destinationInputName: 'Input',
    })
    audioWorkletNode.port.postMessage({
      message: 'connect',
      sourceNodeId: 'Multiplication',
      sourceOutputName: 'Output',
      destinationNodeId: 'NodeProcessor',
      destinationInputName: 'AudioOutput',
    })
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
      const numSamples = 128
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
          numSamples,
          sampleRate,
        },
      )
      offlineAudioWorkletNode.connect(offlineAudioContext.destination)
      addLog('\nRunning test...')
      setupTest(offlineAudioWorkletNode)
      addLog('\nOutput analysis:')
      const renderedBuffer = await offlineAudioContext.startRendering()
      const left = renderedBuffer.getChannelData(0)
      const right = renderedBuffer.getChannelData(1)
      let leftPeak = 0
      let rightPeak = 0
      for (let i = 0; i < 8; i++) {
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
      const precision = 4
      const arrayLeft = []
      for (let i = 0; i < arrayLength; i++) {
        arrayLeft.push(left[i].toFixed(precision))
      }
      const arrayRight = []
      for (let i = 0; i < arrayLength; i++) {
        arrayRight.push(right[i].toFixed(precision))
      }
      addLog(`Peak amplitude (L): ${leftPeak.toFixed(4)}`)
      addLog(`Peak amplitude (R): ${rightPeak.toFixed(4)}`)
      addLog(`First ${arrayLength} samples (L): ${arrayLeft.join(', ')}`)
      addLog(`First ${arrayLength} samples (R): ${arrayRight.join(', ')}`)
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
    } catch (error: any) {
      addLog(`\n❌ Error: ${error.message}`)
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
      audioWorkletNode.connect(audioContext.destination)
      addLog(`\nRunning test...`)
      setupTest(audioWorkletNode)
      const numSeconds = 2
      addLog(`\nPlaying for ${numSeconds} seconds...`)
      await new Promise((resolve) => setTimeout(resolve, numSeconds * 1000))
      addLog('\nStopping...')
      audioWorkletNode.disconnect()
      audioWorkletNode.port.postMessage({ message: 'destroy' })
      setStatus({ message: 'Playback completed', type: 'success' })
      addLog('\n✅ Playback completed')
      setIsPlaying(false)
      setButtonsEnabled({ test: true, play: true })
    } catch (error: any) {
      addLog(`\n❌ Error: ${error.message}`)
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
