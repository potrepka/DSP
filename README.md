# DSP

DSP is an open-source C++ library for audio digital signal processing.

This library is a work in progress. Feedback and contributions are welcome!

## Usage

```cpp
#include "DSP.h"

int main() {

    // TODO: Do something more interesting than passing input to output

    // Setup engine
    std::shared_ptr<dsp::Engine> engine = std::make_shared<dsp::Engine>();
    
    unsigned int inputDevice = engine->getDefaultInputDevice();
    unsigned int outputDevice = engine->getDefaultOutputDevice();
    unsigned int numSamples = 512;
    unsigned int sampleRate = engine->getDefaultSampleRate(inputDevice, outputDevice);
    
    engine->setup(inputDevice, outputDevice, numSamples, sampleRate);
    
    // Create units
    std::shared_ptr<dsp::Identity> identity = std::make_shared<dsp::Identity>();
    identity->setNumChannels(2);
    identity->setNumSamples(engine->getNumSamples());
    identity->setSampleRate(engine->getSampleRate());

    // Push units
    engine->getNodeProcessor()->getNodes().push_back(identity);

    // Connect units
    engine->getNodeProcessor()->getAudioInput() >> identity->getInput();
    identity->getOutput() >> engine->getNodeProcessor()->getAudioOutput();

    return 0;
}
```

## Future Direction

The following are possible directions for future development:
- More filters
- Pitch detection
- Pitch shifting
- Saving state
- Loading state
- Vectorization
- Parallel computing
