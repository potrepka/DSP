#pragma once

#include "Processor.h"

namespace dsp {

class PowerDistortion : public Processor {

public:
    PowerDistortion(Space space = Space::TIME);

    std::shared_ptr<InputParameter> getDrive() const;

protected:
    void process() override;

private:
    const unsigned int DRIVE;
};

} // namespace dsp
