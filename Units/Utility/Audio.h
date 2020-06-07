#pragma once

#include "Unit.h"

namespace dsp {

class Audio : public Unit {

public:
    Audio();
    std::size_t getNumChannels() override;
    std::shared_ptr<OutputParameter> getAudioInput();
    std::shared_ptr<InputParameter> getAudioOutput();
    void readInterleaved(DSP_FLOAT *inputBuffer);
    void writeInterleaved(DSP_FLOAT *outputBuffer);

protected:
    void process() override;
};

} // namespace dsp
