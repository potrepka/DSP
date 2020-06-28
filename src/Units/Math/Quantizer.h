#pragma once

#include "Processor.h"

namespace dsp {

class Quantizer : public Processor {

public:
    Quantizer(Connection::Type type);
    std::shared_ptr<InputParameter> getQuantum();

protected:
    void process() override;

private:
    static const std::size_t QUANTUM;
};

} // namespace dsp
