#pragma once

#include "Processor.h"

namespace dsp {

class EqualTo : public Processor {

public:
    EqualTo(Type type, Space space = Space::TIME);

    std::shared_ptr<InputParameter> getComparisonSignal() const;

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> comparisonSignal;
};

} // namespace dsp
