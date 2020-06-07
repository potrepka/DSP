#pragma once

#include "Filter.h"

namespace dsp {

class Envelope : public Filter {

public:
    Envelope();
    void setNumChannels(std::size_t size) override;
    std::shared_ptr<InputParameter> getAttack();
    std::shared_ptr<InputParameter> getRelease();

protected:
    void process() override;

private:
    static const std::size_t ATTACK;
    static const std::size_t RELEASE;
    std::vector<DSP_FLOAT> outputPrevious;

};

}
