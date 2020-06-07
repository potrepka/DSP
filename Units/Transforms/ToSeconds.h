#pragma once

#include "Filter.h"

namespace dsp {

class ToSeconds : public Filter {

public:
    ToSeconds();

protected:
    void process() override;

};

}
