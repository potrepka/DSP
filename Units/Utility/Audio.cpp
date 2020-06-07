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

void dsp::Audio::readInterleaved(DSP_FLOAT *inputBuffer) {
    lock();
    for (int i = 0; i < getAudioInput()->getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &audioInput = getAudioInput()->getChannel(i)->getBuffer();
        for (int k = 0, sample = i; k < getBufferSize(); k++, sample += getAudioInput()->getNumChannels()) {
            audioInput[k] = inputBuffer[sample];
        }
    }
    unlock();
}

void dsp::Audio::writeInterleaved(DSP_FLOAT *outputBuffer) {
    lock();
    for (int i = 0; i < getAudioOutput()->getNumChannels(); i++) {
        std::vector<DSP_FLOAT> &audioOutput = getAudioOutput()->getChannel(i)->getBuffer();
        for (int k = 0, sample = i; k < getBufferSize(); k++, sample += getAudioOutput()->getNumChannels()) {
            outputBuffer[sample] = audioOutput[k];
        }
    }
    unlock();
}

void dsp::Audio::process() {
    for (const auto &channel : getAudioOutput()->getChannels()) {
        channel->copyBuffers();
    }
}
