# DSP

DSP is a lightweight C++ library for digital signal processing.

This project is still in development. Feedback and contributions are welcome!

## Usage

```cpp
#include "DSP.h"

int main() {
    const unsigned int INPUT_CHANNELS = 2;
    const unsigned int OUTPUT_CHANNELS = 2;
    const unsigned int PASS_CHANNELS = 2;
    const unsigned int SAMPLE_RATE = 44100;
    const unsigned int BUFFER_SIZE = 512;

    std::shared_ptr<dsp::Audio> audio;
    std::shared_ptr<dsp::PassUnit> pass;

    audio = std::make_shared<dsp::Audio>();
    audio->getAudioInput()->setNumChannels(INPUT_CHANNELS);
    audio->getAudioOutput()->setNumChannels(OUTPUT_CHANNELS);
    audio->setSampleRate(SAMPLE_RATE);
    audio->setBufferSize(BUFFER_SIZE);

    // TODO: Do something more interesting than passing input to output
    pass = std::make_shared<dsp::PassUnit>(dsp::Connection::Type::BIPOLAR);
    pass->setNumChannels(PASS_CHANNELS);

    audio->pushUnit(pass);

    // Connect units
    audio->getAudioInput() >> pass->getInputSignal();
    pass->getOutputSignal() >> audio->getAudioOutput();

    // Interleaved buffers
    std::vector<DSP_FLOAT> inputBuffer(INPUT_CHANNELS * BUFFER_SIZE);
    std::vector<DSP_FLOAT> outputBuffer(OUTPUT_CHANNELS * BUFFER_SIZE);

    // Process audio
    audio->zeroBuffers();
    audio->readInterleaved(inputBuffer.data(), INPUT_CHANNELS, BUFFER_SIZE);
    audio->run();
    audio->copyBuffers();
    audio->writeInterleaved(outputBuffer.data(), OUTPUT_CHANNELS, BUFFER_SIZE);
}
```

## Future Direction

The following are possible directions for future development:
- Biquad poles
- FFT
- Convolution
- Variable delay
- Elliptic filter
- Noise generators
- ADSR
- Real-time audio
- MIDI
- Sequencer
- Frequency response
- SIMD instructions
