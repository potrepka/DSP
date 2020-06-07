#pragma once

#include "Generator.h"

namespace dsp {

class SampleRate : public Generator {

public:
    SampleRate();
    void setSampleRate(unsigned int sampleRate);

};

}
