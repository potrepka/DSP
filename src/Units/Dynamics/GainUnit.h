#pragma once

#include "Processor.h"

namespace dsp {

class GainUnit : public Processor {

public:
    GainUnit(Space space = Space::TIME);
    std::shared_ptr<InputParameter> getGain() const;

protected:
    void process() override;

private:
    static const unsigned int GAIN;
};

} // namespace dsp
