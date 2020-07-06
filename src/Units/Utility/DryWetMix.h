#pragma once

#include "Processor.h"

namespace dsp {

class DryWetMix : public Processor {

public:
    DryWetMix(Type type, Space space = Space::TIME);
    std::shared_ptr<InputParameter> getDrySignal();
    std::shared_ptr<InputParameter> getWetSignal();
    std::shared_ptr<InputParameter> getMix();

protected:
    void process() override;

private:
    static const unsigned int DRY_SIGNAL;
    static const unsigned int WET_SIGNAL;
    static const unsigned int MIX;
};

} // namespace dsp
