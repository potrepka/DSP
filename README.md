# DSP

DSP is an open-source C++ library for audio digital signal processing.

## Usage

```cpp
#include "DSP.h"

int main() {
    // Setup engine
    std::shared_ptr<dsp::Engine> engine = std::make_shared<dsp::Engine>();

    unsigned int inputDevice = engine->getDefaultInputDevice();
    unsigned int outputDevice = engine->getDefaultOutputDevice();
    unsigned int numSamples = 128;
    unsigned int sampleRate = engine->getDefaultSampleRate(inputDevice, outputDevice);

    engine->setup(inputDevice, outputDevice, numSamples, sampleRate);

    // Create nodes
    std::shared_ptr<dsp::Phasor> phasor = std::make_shared<dsp::Phasor>();
    std::shared_ptr<dsp::TableOscillator> osc = std::make_shared<dsp::TableOscillator>();
    std::shared_ptr<dsp::Biquad> filter = std::make_shared<dsp::Biquad>();
    std::shared_ptr<dsp::Multiplication> gain = std::make_shared<dsp::Multiplication>();

    phasor->setNumChannels(2);
    osc->setNumChannels(2);
    filter->setNumChannels(2);
    gain->setNumChannels(2);

    // Set input values
    phasor->getFrequency()->setAllChannelValues(55);
    filter->getFrequency()->setAllChannelValues(880);
    filter->getMode()->setAllChannelValues(dsp::BiquadMode::LOW_PASS);
    gain->getFactor()->setAllChannelValues(0.5);

    // Create sawtooth wavetable
    std::shared_ptr<dsp::Buffer> sawtooth = std::make_shared<dsp::Buffer>(
        dsp::Type::RATIO,
        dsp::Space::TIME,
        0.0, // No range clipping
        0.0, // Default value is 0.0
        1,   // 1 channel
        2048 // 2048 samples
    );
    dsp::Sample* bufferChannel = sawtooth->getWrapper().getChannelPointer(0);
    for (size_t sample = 0; sample < sawtooth->getNumSamples(); ++sample) {
        dsp::Sample phase = (dsp::Sample)sample / sawtooth->getNumSamples();
        dsp::Sample value = 2.0 * (fmod(phase + 0.5, 1.0)) - 1.0;
        bufferChannel[sample] = value;
    }
    osc->getTables()->push_back(sawtooth);

    // Add nodes
    engine->getNodeProcessor()->getDefaultNode()->addChild(phasor);
    engine->getNodeProcessor()->getDefaultNode()->addChild(osc);
    engine->getNodeProcessor()->getDefaultNode()->addChild(filter);
    engine->getNodeProcessor()->getDefaultNode()->addChild(gain);

    // Connect the graph
    phasor->getOutput() >> osc->getPhase();
    osc->getOutput() >> filter->getInput();
    filter->getOutput() >> gain->getInput();
    gain->getOutput() >> engine->getNodeProcessor()->getAudioOutput();

    return 0;
}
```

## Future Direction

The following are possible directions for future development:

- Offline buffer rendering for sample player, table oscillator, and sequencer
- Pitch detection, beat detection, pitch shifting, and time stretching nodes
- Graph serialization/deserialization
