#pragma once

#include "../Core/Transformer.h"

#include "../../../lib/FFTConvolver/TwoStageFFTConvolver.h"

namespace dsp {

class Convolver : public Transformer {

public:
    Convolver();

    int getHeadSize() const;
    void setHeadSize(int headSize);
    int getTailSize() const;
    void setTailSize(int tailSize);
    std::shared_ptr<dsp::Buffer> getBuffer(int inChannel, int outChannel) const;
    void setBuffer(int inChannel, int outChannel, std::shared_ptr<Buffer> buffer);
    void initConvolvers();

protected:
    void setNumInputChannelsNoLock(int numChannels) override;
    void setNumOutputChannelsNoLock(int numChannels) override;
    void setNumSamplesNoLock(int numSamples) override;
    void processNoLock() override;

private:
    std::vector<std::vector<std::shared_ptr<Buffer>>> buffers;
    std::vector<std::vector<std::unique_ptr<fftconvolver::TwoStageFFTConvolver>>> convolvers;
    unsigned int headSize;
    unsigned int tailSize;
    std::vector<fftconvolver::Sample> input;
    std::vector<fftconvolver::Sample> output;

    void initConvolver(int inChannel, int outChannel);
};

} // namespace dsp
