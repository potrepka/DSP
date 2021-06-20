#include "ForwardFFT.h"

dsp::ForwardFFT::ForwardFFT()
        : Consumer(Type::RATIO)
        , magnitude(std::make_shared<Output>(Type::RATIO, Space::FREQUENCY))
        , phase(std::make_shared<Output>(Type::RATIO, Space::FREQUENCY)) {
    getOutputs().push_back(magnitude);
    getOutputs().push_back(phase);
}

std::shared_ptr<dsp::Output> dsp::ForwardFFT::getMagnitude() const {
    return magnitude;
}

std::shared_ptr<dsp::Output> dsp::ForwardFFT::getPhase() const {
    return phase;
}

void dsp::ForwardFFT::setNumSamplesNoLock(size_t numSamples) {
    Node::setNumSamplesNoLock(numSamples);
    fft.setup(numSamples);
}

void dsp::ForwardFFT::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *magnitudeChannel = getMagnitude()->getWrapper().getChannelPointer(channel);
        Sample *phaseChannel = getPhase()->getWrapper().getChannelPointer(channel);
        fft.toMagnitudePhase(inputChannel, magnitudeChannel, phaseChannel);
    }
}
