#pragma once

#include "Processor.h"

namespace dsp {

class Floor : public Processor {

public:
    Floor(Connection::Type type, Connection::Space space = Connection::Space::TIME);
    std::shared_ptr<InputParameter> getDivisor();

protected:
    void process() override;

private:
    static const std::size_t DIVISOR;
};

} // namespace dsp
