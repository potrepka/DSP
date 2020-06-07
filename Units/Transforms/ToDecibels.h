#pragma once

#include "Filter.h"

namespace dsp {

class ToDecibels : public Filter {

public:
    ToDecibels();

protected:
    void process() override;

};

}
