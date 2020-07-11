#pragma once

#include "Unit.h"

namespace dsp {

class Audio : public Unit {

public:
    Audio();
    std::shared_ptr<OutputParameter> getAudioInput() const;
    std::shared_ptr<InputParameter> getAudioOutput() const;
    void zeroBuffers();
    void copyBuffers();
    void readInterleaved(DSP_FLOAT *inputBuffer, unsigned int numInputChannels, unsigned int numFrames);
    void writeInterleaved(DSP_FLOAT *outputBuffer, unsigned int numOutputChannels, unsigned int numFrames);
};

} // namespace dsp
