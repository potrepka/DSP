#pragma once

#include "Processor.h"

namespace dsp {

class GainEnvelope : public Processor {

public:
    GainEnvelope();

    std::shared_ptr<InputParameter> getAttack() const;
    std::shared_ptr<InputParameter> getRelease() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> attack;
    const std::shared_ptr<InputParameter> release;

    std::vector<DSP_FLOAT> outputPrevious;
};

} // namespace dsp
