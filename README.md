# DSP

DSP is an open-source C++ library for audio digital signal processing.

This repository is a work in progress. Feedback and contributions are welcome!

## Usage

```cpp
#include "DSP.h"

int main() {

    // TODO: Do something more interesting than passing input to output

    // Setup units
    std::shared_ptr<dsp::PassThrough> pass;

    pass = std::make_shared<dsp::PassThrough>(dsp::Type::RATIO);
    pass->setNumChannels(2);

    engine->getNodeProcessor()->getNodes().push_back(pass);

    // Connect units
    engine->getNodeProcessor()->getAudioInput() >> pass->getInput();
    pass->getOutput() >> engine->getNodeProcessor()->getAudioOutput();

    // Setup engine
    std::shared_ptr<dsp::Engine> engine = std::make_shared<dsp::Engine>();

    unsigned int inputDevice = engine->getDefaultInputDevice();
    unsigned int outputDevice = engine->getDefaultOutputDevice();
    unsigned int numSamples = 512;
    unsigned int sampleRate = engine->getDefaultSampleRate(inputDevice, outputDevice);

    engine->setup(inputDevice, outputDevice, numSamples, sampleRate);

    return 0;
}
```

## Future Direction

The following are possible directions for future development:
- Additional filters
- Frequency response calculation
- Pitch detection
- Pitch shifting
- Saving state
- Loading state
- Parallel computing
