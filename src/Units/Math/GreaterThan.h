#pragma once

#include "Processor.h"

namespace dsp {

class GreaterThan : public Processor {

public:
    GreaterThan(Type type, Space space = Space::TIME);

    std::shared_ptr<InputParameter> getComparisonSignal() const;

protected:
    void process() override;

private:
    const unsigned int COMPARISON_SIGNAL;
};

} // namespace dsp
