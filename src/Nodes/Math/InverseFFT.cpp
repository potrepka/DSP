#include "InverseFFT.h"

dsp::InverseFFT::InverseFFT()
        : Producer(Type::RATIO)
        , magnitude(std::make_shared<Input>(Type::RATIO, Space::FREQUENCY))
        , phase(std::make_shared<Input>(Type::RATIO, Space::FREQUENCY)) {
    getInputs().push_back(magnitude);
    getInputs().push_back(phase);
}

std::shared_ptr<dsp::Input> dsp::InverseFFT::getMagnitude() const {
    return magnitude;
}

std::shared_ptr<dsp::Input> dsp::InverseFFT::getPhase() const {
    return phase;
}

void dsp::InverseFFT::setNumSamplesNoLock(size_t numSamples) {
    Node::setNumSamplesNoLock(numSamples);
    fft.setup(numSamples);
}

void dsp::InverseFFT::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *magnitudeChannel = getMagnitude()->getWrapper().getChannelPointer(channel);
        Sample *phaseChannel = getPhase()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        fft.fromMagnitudePhase(magnitudeChannel, phaseChannel, outputChannel);
    }
}
