#pragma once

#include "../../Core/Producer.h"

#include "../../Core/NormalizedFFT.h"

namespace dsp {

class InverseFFT : public Producer {

public:
    InverseFFT();

    std::shared_ptr<Input> getMagnitude() const;
    std::shared_ptr<Input> getPhase() const;

protected:
    void setNumSamplesNoLock(size_t numSamples) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> magnitude;
    const std::shared_ptr<Input> phase;

    NormalizedFFT fft;
};

} // namespace dsp
