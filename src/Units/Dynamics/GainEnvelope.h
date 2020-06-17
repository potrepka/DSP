#pragma once

#include "Processor.h"

namespace dsp {

class GainEnvelope : public Processor {

public:
    GainEnvelope();
    std::shared_ptr<InputParameter> getAttack();
    std::shared_ptr<InputParameter> getRelease();

protected:
    void setNumChannelsNoLock(std::size_t numChannels) override;
    void process() override;

private:
    static const std::size_t ATTACK;
    static const std::size_t RELEASE;
    std::vector<DSP_FLOAT> outputPrevious;
};

} // namespace dsp
