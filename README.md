# DSP

DSP is an open-source C++ library for real-time digital signal processing.

This project is still in development. Feedback and contributions are welcome!

## Usage

```cpp
#include "DSP.h"

int main() {

    // Setup engine
    dsp::Engine *engine = new dsp::Engine();

    std::vector<unsigned int> inputDevices = engine->getInputDevices();
    std::vector<unsigned int> outputDevices = engine->getOutputDevices();
    unsigned int inputDevice = inputDevices.size() > 0 ? inputDevices[0] : -1;
    unsigned int outputDevice = outputDevices.size() > 0 ? outputDevices[0] : -1;

    std::vector<unsigned int> sampleRates = engine->getAvailableSampleRates(inputDevice, outputDevice);
    unsigned int sampleRate = sampleRates.size() > 0 ? sampleRates[0] : 0;

    unsigned int bufferSize = 512;

    engine->setup(inputDevice, outputDevice, sampleRate, bufferSize);

    // Setup units
    std::shared_ptr<dsp::PassThrough> pass;

    // TODO: Do something more interesting than passing input to output
    pass = std::make_shared<dsp::PassThrough>(dsp::Connection::Type::BIPOLAR);
    pass->setNumChannels(2);

    // Add units
    engine->pushUnit(pass);

    // Connect units
    engine->getAudioInput() >> pass->getInputSignal();
    pass->getOutputSignal() >> engine->getAudioOutput();

    delete engine;

    return 0;
}
```

## Future Direction

The following are possible directions for future development:
- Delay effects
- Elliptic filter
- Crossover filter
- Cascaded filters
- Frequency response calculation
- Pitch detection/shifting
- Peak/RMS detection
- Wavetable factory
- Pattern sequencer
- Saving/Loading state
- SIMD instructions
