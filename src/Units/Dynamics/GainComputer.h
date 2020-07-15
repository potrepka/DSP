#pragma once

#include "Processor.h"

namespace dsp {

class GainComputer : public Processor {

public:
    GainComputer(Space space = Space::TIME);

    std::shared_ptr<InputParameter> getThreshold() const;
    std::shared_ptr<InputParameter> getCompressionRatio() const;
    std::shared_ptr<InputParameter> getGateRatio() const;
    std::shared_ptr<InputParameter> getKnee() const;

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> threshold;
    const std::shared_ptr<InputParameter> compressionRatio;
    const std::shared_ptr<InputParameter> gateRatio;
    const std::shared_ptr<InputParameter> knee;
};

} // namespace dsp
