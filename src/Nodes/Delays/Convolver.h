#pragma once

#include "../Core/Transformer.h"

#include "../../../lib/FFTConvolver/TwoStageFFTConvolver.h"

namespace dsp {

class Convolver : public Transformer {

public:
    Convolver();

    size_t getHeadSize() const;
    void setHeadSize(size_t headSize);
    size_t getTailSize() const;
    void setTailSize(size_t tailSize);
    std::shared_ptr<dsp::Buffer> getBuffer(size_t inChannel, size_t outChannel) const;
    void setBuffer(size_t inChannel, size_t outChannel, std::shared_ptr<Buffer> buffer);
    void initConvolvers();

protected:
    void setNumInputChannelsNoLock(size_t numChannels) override;
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void setNumSamplesNoLock(size_t numSamples) override;
    void processNoLock() override;

private:
    std::vector<std::vector<std::shared_ptr<Buffer>>> buffers;
    std::vector<std::vector<std::unique_ptr<fftconvolver::TwoStageFFTConvolver>>> convolvers;
    size_t headSize;
    size_t tailSize;
    std::vector<fftconvolver::Sample> input;
    std::vector<fftconvolver::Sample> output;

    void initConvolver(size_t inChannel, size_t outChannel);
};

} // namespace dsp
