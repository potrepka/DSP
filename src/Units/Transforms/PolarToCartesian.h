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
    static const unsigned int MAGNITUDE;
    static const unsigned int PHASE;
    static const unsigned int REAL;
    static const unsigned int IMAGINARY;
};

} // namespace dsp
