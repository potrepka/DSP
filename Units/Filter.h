#pragma once

#include "Consumer.h"
#include "Generator.h"

namespace dsp {

class Filter : public Consumer, public Generator {

public:
    Filter(Connection::Type type);
    std::size_t getNumChannels() override;

protected:
    void process() override;

};

}
