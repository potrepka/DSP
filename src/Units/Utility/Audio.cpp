#include "Audio.h"

dsp::Audio::Audio()
        : AUDIO_INPUT(pushOutput(Type::BIPOLAR))
        , AUDIO_OUTPUT(pushInput(Type::BIPOLAR))
        , AUDIO_INPUT_CLIPPING(pushOutput(Type::BINARY))
        , AUDIO_OUTPUT_CLIPPING(pushOutput(Type::BINARY)) {}

std::shared_ptr<dsp::OutputParameter> dsp::Audio::getAudioInput() const {
    return getOutput(AUDIO_INPUT);
}

std::shared_ptr<dsp::InputParameter> dsp::Audio::getAudioOutput() const {
    return getInput(AUDIO_OUTPUT);
}

std::shared_ptr<dsp::OutputParameter> dsp::Audio::getAudioInputClipping() const {
    return getOutput(AUDIO_INPUT_CLIPPING);
}

std::shared_ptr<dsp::OutputParameter> dsp::Audio::getAudioOutputClipping() const {
    return getOutput(AUDIO_OUTPUT_CLIPPING);
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
        std::vector<DSP_FLOAT> &audioInputClipping = getAudioInputClipping()->getChannel(i)->getBuffer();
        for (unsigned int k = 0, sample = i; k < numFrames; k++, sample += numInputChannels) {
            audioInputClipping[k] = abs(inputBuffer[sample]) > 1.0 ? 1.0 : 0.0;
            audioInput[k] = inputBuffer[sample];
        }
    }
    unlock();
}

void dsp::Audio::writeInterleaved(DSP_FLOAT *outputBuffer, unsigned int numOutputChannels, unsigned int numFrames) {
    lock();
    for (unsigned int i = 0; i < numOutputChannels; i++) {
        std::vector<DSP_FLOAT> &audioOutput = getAudioOutput()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &audioOutputClipping = getAudioOutputClipping()->getChannel(i)->getBuffer();
        for (unsigned int k = 0, sample = i; k < numFrames; k++, sample += numOutputChannels) {
            audioOutputClipping[k] = abs(audioOutput[k]) > 1.0 ? 1.0 : 0.0;
            outputBuffer[sample] = clip(audioOutput[k], -1.0, 1.0);
        }
    }
    unlock();
}
