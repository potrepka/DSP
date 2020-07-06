#pragma once

#include "Processor.h"

namespace dsp {

class GainComputer : public Processor {

public:
    GainComputer(Connection::Space space = Connection::Space::TIME);
    std::shared_ptr<InputParameter> getThreshold();
    std::shared_ptr<InputParameter> getCompressionRatio();
    std::shared_ptr<InputParameter> getGateRatio();
    std::shared_ptr<InputParameter> getKnee();

protected:
    void process() override;

private:
    static const unsigned int THRESHOLD;
    static const unsigned int COMPRESSION_RATIO;
    static const unsigned int GATE_RATIO;
    static const unsigned int KNEE;
};

} // namespace dsp
