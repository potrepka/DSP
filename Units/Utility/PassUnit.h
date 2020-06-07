#pragma once

#include "Filter.h"

namespace dsp {

class PassUnit : public Filter {

public:
    PassUnit(Connection::Type type);

protected:
    void process() override;

};

}
