#pragma once

#include "Processor.h"

namespace dsp {

class Divide : public Processor {

public:
    Divide(Connection::Type type);
    std::shared_ptr<InputParameter> getDivisor();

protected:
    void process() override;

private:
    static const std::size_t DIVISOR;
};

} // namespace dsp
