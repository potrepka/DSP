#pragma once

#include "Filter.h"

namespace dsp {

class ToHertz : public Filter {

public:
    ToHertz();

protected:
    void process() override;

};

}
