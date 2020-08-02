# DSP

DSP is an open-source C++ library for audio digital signal processing.

This repository is a work in progress. Feedback and contributions are welcome!

## Usage

```cpp
#include "DSP.h"

int main() {

    // Setup engine
    std::shared_ptr<dsp::Engine> engine = std::make_shared<dsp::Engine>();

    unsigned int inputDevice = engine->getDefaultInputDevice();
    unsigned int outputDevice = engine->getDefaultOutputDevice();
    unsigned int sampleRate = engine->getDefaultSampleRate(inputDevice, outputDevice);
    unsigned int bufferSize = 512;

    engine->setup(inputDevice, outputDevice, sampleRate, bufferSize);

    // TODO: Do something more interesting than passing input to output

    // Setup units
    std::shared_ptr<dsp::PassThrough> pass;

    pass = std::make_shared<dsp::PassThrough>(dsp::Type::BIPOLAR);
    
    pass->setNumChannels(2);

    engine->pushUnit(pass);

    // Connect units
    engine->getAudioInput() >> pass->getInputSignal();
    pass->getOutputSignal() >> engine->getAudioOutput();

    return 0;
}
```

## Future Direction

The following are possible directions for future development:
- Additional filters
- Frequency response calculation
- Pitch detection/shifting
- Saving/Loading state
- Parallel computing
