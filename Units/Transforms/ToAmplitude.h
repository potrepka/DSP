#pragma once

#include "Filter.h"

namespace dsp {

class ToAmplitude : public Filter {

public:
    ToAmplitude();

protected:
    void process() override;

};

}
