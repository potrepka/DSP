#pragma once

#include "Processor.h"

namespace dsp {

class GainComputer : public Processor {

public:
    GainComputer();
    std::shared_ptr<InputParameter> getThreshold();
    std::shared_ptr<InputParameter> getCompressionRatio();
    std::shared_ptr<InputParameter> getGateRatio();
    std::shared_ptr<InputParameter> getKnee();

protected:
    void process() override;

private:
    static const std::size_t THRESHOLD;
    static const std::size_t COMPRESSION_RATIO;
    static const std::size_t GATE_RATIO;
    static const std::size_t KNEE;
};

} // namespace dsp
