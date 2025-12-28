import { initializeWebAudio } from '@potrepka/react-native-dsp'
import { useState, useRef, useEffect } from 'react'

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
  const numChannels = 2
  const bufferSize = 128
  const sampleRate = 48000
  const offlineContextRef = useRef<OfflineAudioContext | null>(null)
  const offlineWorkletRef = useRef<AudioWorkletNode | null>(null)
  const audioContextRef = useRef<AudioContext | null>(null)
  const audioWorkletNodeRef = useRef<AudioWorkletNode | null>(null)
  const addLog = (message: string) => {
    setOutput((prev) => [...prev, message])
  }
  useEffect(() => {
    initialize()
  }, [])
  const initialize = async () => {
    setOutput([])
    setStatus({ message: 'Loading WASM module...', type: 'info' })
    try {
      const { addModule, createAudioWorkletNode } = await initializeWebAudio()
      addLog('Creating OfflineAudioContext...')
      offlineContextRef.current = new OfflineAudioContext(
        numChannels,
        bufferSize,
        sampleRate,
      )
      addLog('Adding module to OfflineAudioContext...')
      await addModule(offlineContextRef.current)
      addLog('Creating AudioWorkletNode in OfflineAudioContext...')
      offlineWorkletRef.current = createAudioWorkletNode(
        offlineContextRef.current,
      )
      offlineWorkletRef.current.connect(offlineContextRef.current.destination)
      addLog('Creating AudioContext...')
      setStatus({ message: 'Creating AudioContext...', type: 'info' })
      audioContextRef.current = new AudioContext({ sampleRate: 48000 })
      setStatus({
        message: 'Adding module to AudioContext...',
        type: 'info',
      })
      await addModule(audioContextRef.current)
      addLog('Creating AudioWorkletNode in AudioContext...')
      audioWorkletNodeRef.current = createAudioWorkletNode(
        audioContextRef.current,
      )
      audioWorkletNodeRef.current.connect(audioContextRef.current.destination)
      setStatus({
        message: 'WASM module loaded successfully!',
        type: 'success',
      })
      setButtonsEnabled({ test: true, play: true })
      addLog('Setup completed')
      addLog('\nOffline Audio Context Info:')
      addLog(`Channels: ${offlineContextRef.current.destination.channelCount}`)
      addLog(`Length: ${offlineContextRef.current.length} samples`)
      addLog(`Sample Rate: ${offlineContextRef.current.sampleRate} Hz`)
      addLog('\nAudio Context Info:')
      addLog(`Channels: ${audioContextRef.current.destination.channelCount}`)
      addLog(`Sample Rate: ${audioContextRef.current.sampleRate} Hz`)
      addLog(`Base Latency: ${audioContextRef.current.baseLatency} seconds`)
      addLog(`Output Latency: ${audioContextRef.current.outputLatency} seconds`)
    } catch (error: any) {
      setStatus({
        message: `Failed to initialize: ${error.message}`,
        type: 'error',
      })
      addLog(`❌ Error: ${error.message}`)
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
    setTimeout(() => {
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
        outputNodeId: 'Phasor',
        outputName: 'Output',
        inputNodeId: 'Multiplication',
        inputName: 'Input',
      })
      audioWorkletNode.port.postMessage({
        message: 'connect',
        outputNodeId: 'Multiplication',
        outputName: 'Output',
        inputNodeId: 'NodeProcessor',
        inputName: 'AudioOutput',
      })
    }, 50)
  }
  const runTest = async () => {
    if (!offlineContextRef.current || !offlineWorkletRef.current) {
      return
    }
    setOutput([])
    addLog('Running test...')
    try {
      setupTest(offlineWorkletRef.current)
      addLog('\nOutput analysis:')
      const renderedBuffer = await offlineContextRef.current.startRendering()
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
        addLog('\n✅ Test completed successfully (signal detected)')
      } else {
        addLog('\n⚠️ Test completed (silence detected)')
      }
    } catch (error: any) {
      addLog(`❌ Error: ${error.message}`)
      console.error(error)
    }
  }
  const playTest = async () => {
    if (!audioContextRef.current || !audioWorkletNodeRef.current || isPlaying) {
      return
    }
    setOutput([])
    if (audioContextRef.current.state === 'suspended') {
      await audioContextRef.current.resume()
    }
    setIsPlaying(true)
    setButtonsEnabled({ test: false, play: false })
    try {
      const sampleRate = audioContextRef.current.sampleRate
      addLog(`Testing playback at ${sampleRate} Hz...`)
      setupTest(audioWorkletNodeRef.current)
      addLog('\nPlaying...')
      await new Promise((resolve) => setTimeout(resolve, 2000))
      addLog('\nStopping...')
      audioWorkletNodeRef.current.disconnect()
      audioWorkletNodeRef.current.port.postMessage({ message: 'destroy' })
      addLog('\n✅ Playback completed')
      setIsPlaying(false)
      setButtonsEnabled({ test: true, play: true })
    } catch (error: any) {
      addLog(`❌ Error: ${error.message}`)
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
              : status.type === 'error'
              ? '#f8d7da'
              : '#d1ecf1',
          color:
            status.type === 'success'
              ? '#155724'
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
          onClick={runTest}
          disabled={!buttonsEnabled.test}
          style={{
            padding: '10px 20px',
            fontSize: '16px',
            cursor: 'pointer',
          }}
        >
          Run Test
        </button>
        <button
          onClick={playTest}
          disabled={!buttonsEnabled.play}
          style={{
            padding: '10px 20px',
            fontSize: '16px',
            cursor: 'pointer',
          }}
        >
          Start Engine & Play
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
