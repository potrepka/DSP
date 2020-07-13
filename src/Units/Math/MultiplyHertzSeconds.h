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
    static const unsigned int FREQUENCY;
    static const unsigned int TIME;
};

} // namespace dsp
