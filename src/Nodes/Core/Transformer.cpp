#include "Transformer.h"

dsp::Transformer::Transformer(Type inType, Type outType, Space space)
        : Node()
        , Consumer(inType, space)
        , Producer(outType, space) {}

void dsp::Transformer::transform(std::function<Sample(Sample)> transform) {
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getBlock().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getBlock().getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            outputChannel[sample] = transform(inputChannel[sample]);
        }
    }
}

void dsp::Transformer::transform(std::shared_ptr<Input> parameter, std::function<Sample(Sample, Sample)> transform) {
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getBlock().getChannelPointer(channel);
        Sample *parameterChannel = parameter->getBlock().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getBlock().getChannelPointer(channel);
        for (int sample = 0; sample < getNumSamples(); ++sample) {
            outputChannel[sample] = transform(inputChannel[sample], parameterChannel[sample]);
        }
    }
}
