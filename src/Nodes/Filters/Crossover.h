#pragma once

#include "../Math/PassThrough.h"
#include "Biquad.h"

namespace dsp {

class Crossover : public Node {

public:
    Crossover();

    std::shared_ptr<Input> getInput();
    std::shared_ptr<Input> getFrequency();
    std::shared_ptr<Output> getLow();
    std::shared_ptr<Output> getHigh();

protected:
    void setNumInputChannelsNoLock(size_t numChannels) override;
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<PassThrough> input;
    const std::shared_ptr<PassThrough> frequency;
    const std::shared_ptr<Biquad> lp1;
    const std::shared_ptr<Biquad> lp2;
    const std::shared_ptr<Biquad> hp1;
    const std::shared_ptr<Biquad> hp2;
};

} // namespace dsp
