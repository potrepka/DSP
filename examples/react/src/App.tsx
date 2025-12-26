import { useState, useRef, useEffect } from 'react'
import { createRoot } from 'react-dom/client'
import type { AudioGraphNode } from '@potrepka/react-native-dsp/dist/classes/AudioGraphNode'
import type { TestResult } from '@potrepka/react-native-dsp/dist/classes/AudioGraphNode'

const App = () => {
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
  const audioContextRef = useRef<AudioContext | null>(null)
  const audioNodeRef = useRef<AudioGraphNode | null>(null)
  useEffect(() => {
    initialize()
  }, [])
  const initialize = async () => {
    try {
      setStatus({ message: 'Initializing AudioContext...', type: 'info' })
      audioContextRef.current = new AudioContext({ sampleRate: 48000 })
      setStatus({ message: 'Loading WASM module...', type: 'info' })
      const { AudioGraphNode: AudioGraphNodeClass } = await import(
        '@potrepka/react-native-dsp/dist/classes/AudioGraphNode'
      )
      audioNodeRef.current = await AudioGraphNodeClass.create(
        audioContextRef.current,
        {
          wasmModuleUrl: `${window.location.origin}/node_modules/@potrepka/react-native-dsp/web/build/react-native-dsp.js`,
          processorUrl: `${window.location.origin}/node_modules/@potrepka/react-native-dsp/dist/classes/AudioGraphProcessor.js`,
        },
      )
      const info = await audioNodeRef.current.getModuleInfo()
      setStatus({
        message: 'WASM module loaded successfully!',
        type: 'success',
      })
      setButtonsEnabled({ test: true, play: true })
      addLog('WASM module initialized')
      addLog(`\nAvailable constants: ${info.constants.join(', ')}`)
      addLog(`\nAvailable functions: ${info.functions.join(', ')}`)
    } catch (error: any) {
      setStatus({
        message: `Failed to initialize: ${error.message}`,
        type: 'error',
      })
      addLog(`❌ Error: ${error.message}`)
      console.error('Initialization error:', error)
    }
  }
  const addLog = (message: string) => {
    setOutput((prev) => [...prev, message])
  }
  const runTest = async () => {
    if (!audioNodeRef.current) {
      return
    }
    setOutput([])
    addLog('Running test...')
    const sampleRate = 48000
    const numChannels = 2
    const bufferSize = 256
    addLog(
      `\nSetting up Engine...\nSample Rate: ${sampleRate} Hz\nChannels: ${numChannels}\nBuffer Size: ${bufferSize} samples`,
    )
    try {
      const result: TestResult = await audioNodeRef.current.runTest({
        sampleRate,
        numChannels,
        bufferSize,
      })
      addLog('\nOutput analysis:')
      addLog(`Peak amplitude (L): ${result.leftPeak.toFixed(4)}`)
      addLog(`Peak amplitude (R): ${result.rightPeak.toFixed(4)}`)
      addLog(
        `First ${result.arrayLength} samples (L): ${result.arrayLeft.join(
          ', ',
        )}`,
      )
      addLog(
        `First ${result.arrayLength} samples (R): ${result.arrayRight.join(
          ', ',
        )}`,
      )
      if (result.leftPeak > 0 || result.rightPeak > 0) {
        addLog('\n✅ Test completed successfully (signal detected)')
      } else {
        addLog('\n⚠️ Test completed (silence detected)')
      }
    } catch (error: any) {
      addLog(`❌ Error: ${error.message}`)
      console.error('Test error:', error)
    }
  }
  const playTest = async () => {
    if (!audioNodeRef.current || !audioContextRef.current || isPlaying) {
      return
    }
    try {
      if (audioContextRef.current.state === 'suspended') {
        await audioContextRef.current.resume()
      }
      setIsPlaying(true)
      setButtonsEnabled({ test: false, play: false })
      setOutput([])
      const sampleRate = audioContextRef.current.sampleRate
      const numChannels = 2
      const bufferSize = 128
      addLog(`Testing playback at ${sampleRate} Hz...`)
      addLog(
        `\nSetting up Engine...\nSample Rate: ${sampleRate} Hz\nChannels: ${numChannels}\nBuffer Size: ${bufferSize} samples`,
      )
      await audioNodeRef.current.setupEngine({
        sampleRate,
        numChannels,
        bufferSize,
      })
      addLog('\nEngine setup complete')
      addLog('\nPlaying...')
      audioNodeRef.current.connect(audioContextRef.current.destination)
      await new Promise((resolve) => setTimeout(resolve, 2000))
      addLog('\nStopping...')
      audioNodeRef.current.disconnect()
      await audioNodeRef.current.cleanup()
      addLog('\n✅ Playback completed')
      setIsPlaying(false)
      setButtonsEnabled({ test: true, play: true })
    } catch (error: any) {
      addLog(`❌ Error: ${error.message}`)
      console.error('Play error:', error)
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

const root = createRoot(document.getElementById('root')!)
root.render(<App />)
