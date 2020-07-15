#pragma once

#include "Unit.h"

namespace dsp {

class CartesianToPolar : public Unit {

public:
    CartesianToPolar();

    std::shared_ptr<InputParameter> getReal() const;
    std::shared_ptr<InputParameter> getImaginary() const;
    std::shared_ptr<OutputParameter> getMagnitude() const;
    std::shared_ptr<OutputParameter> getPhase() const;

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> real;
    const std::shared_ptr<InputParameter> imaginary;

    const std::shared_ptr<OutputParameter> magnitude;
    const std::shared_ptr<OutputParameter> phase;
};

} // namespace dsp
