#include "Audio.h"

dsp::Audio::Audio() {
    pushOutput(Connection::Type::BIPOLAR);
    pushInput(Connection::Type::BIPOLAR);
}

std::size_t dsp::Audio::getNumChannels() {
    std::size_t inputChannels = getAudioInput()->getNumChannels();
    std::size_t outputChannels = getAudioOutput()->getNumChannels();
    return inputChannels < outputChannels ? inputChannels : outputChannels;
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::Audio::getAudioInput() {
    return getOutput(0);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Audio::getAudioOutput() {
    return getInput(0);
}

void dsp::Audio::process() {
    for (const auto& channel : getAudioOutput()->getChannels()) {
        channel->copyBuffers();
    }
}
