#pragma once

#include "Buffer.h"
#include "Processor.h"
#include "TwoStageFFTConvolver.h"

namespace dsp {

class Convolver : public Processor {

public:
    Convolver();

    std::shared_ptr<Buffer> getBuffer(unsigned int channel) const;
    void setBuffer(unsigned int channel, std::shared_ptr<Buffer> buffer);
    unsigned int getHeadSize() const;
    unsigned int getTailSize() const;
    void setTailSize(unsigned int size);
    void setHeadSize(unsigned int size);
    void initConvolvers();

protected:
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    std::vector<std::shared_ptr<Buffer>> buffers;
    std::vector<std::vector<std::unique_ptr<fftconvolver::TwoStageFFTConvolver>>> convolvers;
    unsigned int headSize;
    unsigned int tailSize;
    std::vector<float> input;
    std::vector<float> output;

    void initConvolver(unsigned int channel);
};

} // namespace dsp
