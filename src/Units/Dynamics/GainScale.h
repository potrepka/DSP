#pragma once

#include "Processor.h"

namespace dsp {

class GainScale : public Processor {

public:
    GainScale(Type type, Space space = Space::TIME);

    std::shared_ptr<InputParameter> getGain() const;

protected:
    void process() override;

private:
    const unsigned int GAIN;
};

} // namespace dsp
