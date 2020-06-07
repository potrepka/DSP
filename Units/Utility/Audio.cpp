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

void dsp::Audio::zero() {
    lock();
    for (const auto &channel : getAudioInput()->getChannels()) {
        channel->fillBuffer(0);
    }
    unlock();
}

void dsp::Audio::readInterleaved(DSP_FLOAT *inputBuffer, unsigned int numInputChannels, unsigned int numFrames) {
    lock();
    for (int i = 0; i < numInputChannels; i++) {
        std::vector<DSP_FLOAT> &audioInput = getAudioInput()->getChannel(i)->getBuffer();
        for (int k = 0, sample = i; k < numFrames; k++, sample += numInputChannels) {
            audioInput[k] = inputBuffer[sample];
        }
    }
    unlock();
}

void dsp::Audio::writeInterleaved(DSP_FLOAT *outputBuffer, unsigned int numOutputChannels, unsigned int numFrames) {
    lock();
    for (int i = 0; i < numOutputChannels; i++) {
        std::vector<DSP_FLOAT> &audioOutput = getAudioOutput()->getChannel(i)->getBuffer();
        for (int k = 0, sample = i; k < numFrames; k++, sample += numOutputChannels) {
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
