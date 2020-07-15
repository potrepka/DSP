#pragma once

#include "Generator.h"

namespace dsp {

class MultiplyHertzSeconds : public Generator {

public:
    MultiplyHertzSeconds(Space space = Space::TIME);

    std::shared_ptr<InputParameter> getFrequency() const;
    std::shared_ptr<InputParameter> getTime() const;

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> frequency;
    const std::shared_ptr<InputParameter> time;
};

} // namespace dsp
