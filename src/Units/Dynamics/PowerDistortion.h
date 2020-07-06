#pragma once

#include "Processor.h"

namespace dsp {

class PowerDistortion : public Processor {

public:
    PowerDistortion(Connection::Space space = Connection::Space::TIME);
    std::shared_ptr<InputParameter> getDrive();

protected:
    void process() override;

private:
    static const unsigned int DRIVE;
};

} // namespace dsp
