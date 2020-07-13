#pragma once

#include "Processor.h"

namespace dsp {

class DryWetMix : public Processor {

public:
    DryWetMix(Type type, Space space = Space::TIME);

    std::shared_ptr<InputParameter> getDrySignal() const;
    std::shared_ptr<InputParameter> getWetSignal() const;
    std::shared_ptr<InputParameter> getMixAmount() const;

protected:
    void process() override;

private:
    static const unsigned int DRY_SIGNAL;
    static const unsigned int WET_SIGNAL;
    static const unsigned int MIX_AMOUNT;
};

} // namespace dsp
