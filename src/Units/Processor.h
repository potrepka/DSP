#pragma once

#include "Consumer.h"
#include "Generator.h"

namespace dsp {

class Processor : public Consumer, public Generator {

public:
    Processor(Type inType, Type outType, Space space = Space::TIME);

protected:
    void transform(std::function<DSP_FLOAT(DSP_FLOAT)> transform);
    void transform(std::shared_ptr<InputParameter> input, std::function<DSP_FLOAT(DSP_FLOAT, DSP_FLOAT)> transform);
};

} // namespace dsp
