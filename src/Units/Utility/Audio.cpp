#include "Audio.h"

dsp::Audio::Audio() {
    pushOutput(Type::BIPOLAR);
    pushInput(Type::BIPOLAR);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::Audio::getAudioInput() const {
    return getOutput(0);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Audio::getAudioOutput() const {
    return getInput(0);
}

void dsp::Audio::zeroBuffers() {
    lock();
    for (const auto &channel : getAudioInput()->getChannels()) {
        channel->fillBuffer(0.0);
    }
    unlock();
}

void dsp::Audio::copyBuffers() {
    lock();
    for (const auto &channel : getAudioOutput()->getChannels()) {
        channel->copyBuffers();
    }
    unlock();
}

void dsp::Audio::readInterleaved(DSP_FLOAT *inputBuffer, unsigned int numInputChannels, unsigned int numFrames) {
    lock();
    for (unsigned int i = 0; i < numInputChannels; i++) {
        std::vector<DSP_FLOAT> &audioInput = getAudioInput()->getChannel(i)->getBuffer();
        for (unsigned int k = 0, sample = i; k < numFrames; k++, sample += numInputChannels) {
            audioInput[k] = inputBuffer[sample];
        }
    }
    unlock();
}

void dsp::Audio::writeInterleaved(DSP_FLOAT *outputBuffer, unsigned int numOutputChannels, unsigned int numFrames) {
    lock();
    for (unsigned int i = 0; i < numOutputChannels; i++) {
        std::vector<DSP_FLOAT> &audioOutput = getAudioOutput()->getChannel(i)->getBuffer();
        for (unsigned int k = 0, sample = i; k < numFrames; k++, sample += numOutputChannels) {
            outputBuffer[sample] = clip(audioOutput[k], -1.0, 1.0);
        }
    }
    unlock();
}
