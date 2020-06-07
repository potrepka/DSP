#pragma once

#include "Filter.h"

namespace dsp {

class GainUnit : public Filter {

public:
    GainUnit();
    std::shared_ptr<InputParameter> getGain();

protected:
    void process() override;

private:
    static const std::size_t GAIN;

};

}
