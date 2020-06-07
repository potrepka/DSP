#pragma once

#include "Filter.h"

namespace dsp {

class DryWetMix : public Filter {

public:
    DryWetMix();
    std::shared_ptr<InputParameter> getDrySignal();
    std::shared_ptr<InputParameter> getWetSignal();
    std::shared_ptr<InputParameter> getMix();

protected:
    void process() override;

private:
    static const std::size_t DRY_SIGNAL;
    static const std::size_t WET_SIGNAL;
    static const std::size_t MIX;
};

} // namespace dsp
