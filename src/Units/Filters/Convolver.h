#pragma once

#include "Processor.h"
#include "TwoStageFFTConvolver.h"

namespace dsp {

class Convolver : public Processor {

public:
    Convolver();
    std::vector<DSP_FLOAT> getSample(std::size_t channel);
    void setSample(std::size_t channel, const std::vector<DSP_FLOAT> &sample);
    std::size_t getHeadSize();
    std::size_t getTailSize();
    void setTailSize(std::size_t size);
    void setHeadSize(std::size_t size);

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void setNumChannelsNoLock(std::size_t numChannels) override;
    void process() override;

private:
    std::vector<std::vector<DSP_FLOAT>> samples;
    std::vector<std::unique_ptr<fftconvolver::TwoStageFFTConvolver>> convolvers;
    std::size_t headSize;
    std::size_t tailSize;
    std::vector<float> input;
    std::vector<float> output;
    void initConvolver(std::size_t channel);
};

} // namespace dsp
