#pragma once

#include "Unit.h"

namespace dsp {

class Generator : virtual public Unit {

public:
    Generator(Type type, Space space = Space::TIME);

    std::shared_ptr<OutputParameter> getOutputSignal() const;
    void setOutputSignal(std::shared_ptr<OutputParameter> output);

protected:
#if DSP_USE_VC
    void transformOutput(std::shared_ptr<InputParameter> parameter, std::function<Vector(Vector, Vector)> transform);
#else
    void transformOutput(std::shared_ptr<InputParameter> parameter, std::function<Sample(Sample, Sample)> transform);
#endif

private:
    std::shared_ptr<OutputParameter> outputSignal;
};

} // namespace dsp
