#pragma once

#include "Unit.h"

namespace dsp {

class PolarToCartesian : public Unit {

public:
    PolarToCartesian();

    std::shared_ptr<InputParameter> getMagnitude() const;
    std::shared_ptr<InputParameter> getPhase() const;
    std::shared_ptr<OutputParameter> getReal() const;
    std::shared_ptr<OutputParameter> getImaginary() const;

protected:
    void process() override;

private:
    const unsigned int MAGNITUDE;
    const unsigned int PHASE;
    const unsigned int REAL;
    const unsigned int IMAGINARY;
};

} // namespace dsp
