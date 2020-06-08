#pragma once

#include "Processor.h"

namespace dsp {

class GainUnit : public Processor {

public:
    GainUnit();
    std::shared_ptr<InputParameter> getGain();

protected:
    void process() override;

private:
    static const std::size_t GAIN;
};

} // namespace dsp
