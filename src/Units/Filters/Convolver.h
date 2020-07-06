#pragma once

#include "Processor.h"
#include "TwoStageFFTConvolver.h"

namespace dsp {

class Convolver : public Processor {

public:
    Convolver();
    std::vector<DSP_FLOAT> getSample(unsigned int channel);
    void setSample(unsigned int channel, const std::vector<DSP_FLOAT> &sample);
    unsigned int getHeadSize();
    unsigned int getTailSize();
    void setTailSize(unsigned int size);
    void setHeadSize(unsigned int size);

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    std::vector<std::vector<DSP_FLOAT>> samples;
    std::vector<std::unique_ptr<fftconvolver::TwoStageFFTConvolver>> convolvers;
    unsigned int headSize;
    unsigned int tailSize;
    std::vector<float> input;
    std::vector<float> output;
    void initConvolver(unsigned int channel);
};

} // namespace dsp
