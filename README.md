# DSP

DSP is an open-source C++ library for audio digital signal processing.

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

    // Create unit
    std::shared_ptr<dsp::Identity> myNode = std::make_shared<dsp::Identity>();
    myNode->setNumChannels(2);

    // Add unit
    engine->getNodeProcessor()->getDefaultNode()->addChild(myNode);

    // Connect unit
    engine->getNodeProcessor()->getAudioInput() >> myNode->getInput();
    myNode->getOutput() >> engine->getNodeProcessor()->getAudioOutput();

    return 0;
}
```

## Future Direction

The following are possible directions for future development:
- NRT buffer system for sample player and table oscillator
- Saving state
- Loading state
- One-pole frequency response
- Pitch detection
- Pitch shifting
- Additional filters
