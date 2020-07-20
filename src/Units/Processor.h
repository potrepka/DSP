#pragma once

#include "Consumer.h"
#include "Generator.h"

namespace dsp {

class Processor : public Consumer, public Generator {

public:
    Processor(Type inType, Type outType, Space space = Space::TIME);

protected:
#if DSP_USE_VC
    void transform(std::function<Vector(Vector)> transform);
    void transform(std::shared_ptr<InputParameter> parameter, std::function<Vector(Vector, Vector)> transform);
#else
    void transform(std::function<Sample(Sample)> transform);
    void transform(std::shared_ptr<InputParameter> parameter, std::function<Sample(Sample, Sample)> transform);
#endif
};

} // namespace dsp
