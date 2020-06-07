#pragma once

#include "Filter.h"

namespace dsp {

class AbsoluteValue : public Filter {

public:
    AbsoluteValue();

private:
    void process() override;

};

}
