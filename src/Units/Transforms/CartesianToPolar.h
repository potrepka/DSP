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
    static const unsigned int REAL;
    static const unsigned int IMAGINARY;
    static const unsigned int MAGNITUDE;
    static const unsigned int PHASE;
};

} // namespace dsp
