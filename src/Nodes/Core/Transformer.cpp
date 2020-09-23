#include "Transformer.h"

dsp::Transformer::Transformer(Type inType, Type outType, Space space)
        : Node()
        , Consumer(inType, space)
        , Producer(outType, space) {}

dsp::Transformer::Transformer(Type inType, Type outType, Space inSpace, Space outSpace)
        : Node()
        , Consumer(inType, inSpace)
        , Producer(outType, outSpace) {}

void dsp::Transformer::transform(std::function<Sample(Sample)> transform) {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            outputChannel[sample] = transform(inputChannel[sample]);
        }
    }
}

void dsp::Transformer::transform(std::shared_ptr<Input> parameter, std::function<Sample(Sample, Sample)> transform) {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *parameterChannel = parameter->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            outputChannel[sample] = transform(inputChannel[sample], parameterChannel[sample]);
        }
    }
}
