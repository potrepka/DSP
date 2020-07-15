#pragma once

#include "Unit.h"

namespace dsp {

class Audio : public Unit {

public:
    Audio();

    std::shared_ptr<OutputParameter> getAudioInput() const;
    std::shared_ptr<InputParameter> getAudioOutput() const;
    std::shared_ptr<OutputParameter> getAudioInputClipping() const;
    std::shared_ptr<OutputParameter> getAudioOutputClipping() const;
    void zeroBuffers();
    void copyBuffers();
    void readInterleaved(DSP_FLOAT *inputBuffer, unsigned int numInputChannels, unsigned int numFrames);
    void writeInterleaved(DSP_FLOAT *outputBuffer, unsigned int numOutputChannels, unsigned int numFrames);

private:
    const std::shared_ptr<OutputParameter> audioInput;
    const std::shared_ptr<InputParameter> audioOutput;
    const std::shared_ptr<OutputParameter> audioInputClipping;
    const std::shared_ptr<OutputParameter> audioOutputClipping;
};

} // namespace dsp
