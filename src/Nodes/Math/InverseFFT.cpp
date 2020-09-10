#include "InverseFFT.h"

dsp::InverseFFT::InverseFFT()
        : Producer(Type::RATIO)
        , magnitude(std::make_shared<Input>(Type::RATIO, Space::FREQUENCY))
        , phase(std::make_shared<Input>(Type::RATIO, Space::FREQUENCY)) {}

std::shared_ptr<dsp::Input> dsp::InverseFFT::getMagnitude() const {
    return magnitude;
}

std::shared_ptr<dsp::Input> dsp::InverseFFT::getPhase() const {
    return phase;
}

void dsp::InverseFFT::setNumSamplesNoLock(int numSamples) {
    Node::setNumSamplesNoLock(numSamples);
    fft.setup(numSamples);
}

void dsp::InverseFFT::processNoLock() {
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *magnitudeChannel = getMagnitude()->getWrapper().getChannelPointer(channel);
        Sample *phaseChannel = getPhase()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        fft.fromMagnitudePhase(magnitudeChannel, phaseChannel, outputChannel);
    }
}
