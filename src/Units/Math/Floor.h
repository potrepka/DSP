#pragma once

#include "Processor.h"

namespace dsp {

class Floor : public Processor {

public:
    Floor(Type type, Space space = Space::TIME);

    std::shared_ptr<InputParameter> getDivisor() const;

protected:
    void process() override;

private:
    static const unsigned int DIVISOR;
};

} // namespace dsp
