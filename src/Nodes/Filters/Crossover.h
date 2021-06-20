#pragma once

#include "../Math/Identity.h"
#include "Biquad.h"

namespace dsp {

class Crossover : public Node {

public:
    Crossover();

    std::shared_ptr<Input> getInput() const;
    std::shared_ptr<Input> getFrequency() const;
    std::shared_ptr<Output> getLow() const;
    std::shared_ptr<Output> getHigh() const;

protected:
    void setNumInputChannelsNoLock(size_t numChannels) override;
    void setNumOutputChannelsNoLock(size_t numChannels) override;

private:
    const std::shared_ptr<Identity> input;
    const std::shared_ptr<Identity> frequency;
    const std::shared_ptr<Biquad> lp1;
    const std::shared_ptr<Biquad> lp2;
    const std::shared_ptr<Biquad> hp1;
    const std::shared_ptr<Biquad> hp2;
};

} // namespace dsp
