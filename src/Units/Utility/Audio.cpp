#include "Audio.h"

dsp::Audio::Audio()
        : audioOutput(pushInput(Type::BIPOLAR))
        , audioInput(pushOutput(Type::BIPOLAR))
        , audioInputClipping(pushOutput(Type::BINARY))
        , audioOutputClipping(pushOutput(Type::BINARY)) {}

std::shared_ptr<dsp::InputParameter> dsp::Audio::getAudioOutput() const {
    return audioOutput;
}

std::shared_ptr<dsp::OutputParameter> dsp::Audio::getAudioInput() const {
    return audioInput;
}

std::shared_ptr<dsp::OutputParameter> dsp::Audio::getAudioInputClipping() const {
    return audioInputClipping;
}

std::shared_ptr<dsp::OutputParameter> dsp::Audio::getAudioOutputClipping() const {
    return audioOutputClipping;
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
        std::vector<DSP_FLOAT> &audioInputBuffer = getAudioInput()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &audioInputClippingBuffer = getAudioInputClipping()->getChannel(i)->getBuffer();
        for (unsigned int k = 0, sample = i; k < numFrames; k++, sample += numInputChannels) {
            audioInputClippingBuffer[k] = abs(inputBuffer[sample]) > 1.0 ? 1.0 : 0.0;
            audioInputBuffer[k] = inputBuffer[sample];
        }
    }
    unlock();
}

void dsp::Audio::writeInterleaved(DSP_FLOAT *outputBuffer, unsigned int numOutputChannels, unsigned int numFrames) {
    lock();
    for (unsigned int i = 0; i < numOutputChannels; i++) {
        std::vector<DSP_FLOAT> &audioOutputBuffer = getAudioOutput()->getChannel(i)->getBuffer();
        std::vector<DSP_FLOAT> &audioOutputClippingBuffer = getAudioOutputClipping()->getChannel(i)->getBuffer();
        for (unsigned int k = 0, sample = i; k < numFrames; k++, sample += numOutputChannels) {
            audioOutputClippingBuffer[k] = abs(audioOutputBuffer[k]) > 1.0 ? 1.0 : 0.0;
            outputBuffer[sample] = clip(audioOutputBuffer[k], -1.0, 1.0);
        }
    }
    unlock();
}
