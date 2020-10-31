#include "Lag.h"

dsp::Lag::Lag(Type type)
        : Transformer(type, type)
        , lagTime(std::make_shared<Input>(Type::SECONDS)) {
    getInputs().push_back(lagTime);
}

std::shared_ptr<dsp::Input> dsp::Lag::getLagTime() const {
    return lagTime;
}

void dsp::Lag::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    state.resize(numChannels, 0.0);
}

void dsp::Lag::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *lagTimeChannel = getLagTime()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (isnan(state[channel])) {
                state[channel] = 0.0;
            }
            Sample lagSamples = lagTimeChannel[sample] * getSampleRate();
            state[channel] =
                    inputChannel[sample] + pow(0.001, 1.0 / lagSamples) * (state[channel] - inputChannel[sample]);
            outputChannel[sample] = state[channel];
        }
    }
}
