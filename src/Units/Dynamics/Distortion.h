#pragma once

#include "Processor.h"

namespace dsp {

class Distortion : public Processor {

public:
    Distortion(Space space = Space::TIME);

    std::shared_ptr<InputParameter> getDrive() const;

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> drive;
};

} // namespace dsp
