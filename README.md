# DSP

DSP is a lightweight C++ library for digital signal processing. The project is
still in development.

Feedback and contributions are welcome!

## Usage

```cpp
#include "DSP.h"

int main() {
    const unsigned int NUM_INPUT_CHANNELS = 2;
    const unsigned int NUM_OUTPUT_CHANNELS = 2;
    const unsigned int SAMPLE_RATE = 44100;
    const unsigned int BUFFER_SIZE = 512;

    dsp::Audio* audio = new dsp::Audio();
    audio->getAudioInput()->setNumChannels(NUM_CHANNELS);
    audio->getAudioOutput()->setNumChannels(NUM_CHANNELS);
    audio->setSampleRate(SAMPLE_RATE);
    audio->setBufferSize(BUFFER_SIZE);

    dsp::PassUnit* unit = new dsp::PassUnit(dsp::Connection::Type::BIPOLAR);
    unit->setNumChannels(NUM_INPUT_CHANNELS);
    unit->setSampleRate(SAMPLE_RATE);
    unit->setBufferSize(BUFFER_SIZE);

    *data->audio->getAudioInput() >> *unit->getInputSignal();
    *unit->getOutputSignal() >> *data->audio->getAudioOutput();

    data->audio->pushUnit(unit);

    // INTERLEAVED
    vector<DSP_FLOAT>& inputBuffer(NUM_INPUT_CHANNELS * BUFFER_SIZE);
    vector<DSP_FLOAT>& outputBuffer(NUM_OUTPUT_CHANNELS * BUFFER_SIZE);

    for (int i = 0; i < NUM_INPUT_CHANNELS; i++) {
        std::vector<DSP_FLOAT>& input = audio->getAudioInput()->getChannel(i)->getBuffer();
        for (int frame = 0, sample = i; frame < BUFFER_SIZE; frame++, sample += NUM_INPUT_CHANNELS) {
            input[frame] = inputBuffer[sample];
        }
    }

    data->audio->run();

    for (int i = 0; i < NUM_OUTPUT_CHANNELS; i++) {
        std::vector<DSP_FLOAT>& output = audio->getAudioOutput()->getChannel(i)->getBuffer();
        for (int frame = 0, sample = i; frame < BUFFER_SIZE; frame++, sample += NUM_OUTPUT_CHANNELS) {
            outputBuffer[sample] = output[frame];
        }
    }
}
```

## Future Direction

The following are possible directions for future development:
- FFT
- Convolution
- Elliptic filter
- Noise generators
- Real-time audio
- MIDI
- Frequency response
- SIMD instructions
