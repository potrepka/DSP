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
    const std::shared_ptr<InputParameter> magnitude;
    const std::shared_ptr<InputParameter> phase;

    const std::shared_ptr<OutputParameter> real;
    const std::shared_ptr<OutputParameter> imaginary;
};

} // namespace dsp
