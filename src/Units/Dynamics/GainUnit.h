#pragma once

#include "Processor.h"

namespace dsp {

class GainUnit : public Processor {

public:
    GainUnit(Connection::Space space = Connection::Space::TIME);
    std::shared_ptr<InputParameter> getGain();

protected:
    void process() override;

private:
    static const unsigned int GAIN;
};

} // namespace dsp
