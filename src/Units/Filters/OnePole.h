#pragma once

#include "Processor.h"

namespace dsp {

class OnePole : public Processor {

public:
    enum class Mode { LOW_PASS, HIGH_PASS };

    OnePole();
    Mode getMode();
    void setMode(Mode mode);
    std::shared_ptr<InputParameter> getFrequency();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const unsigned int FREQUENCY;
    Mode mode;
    std::vector<DSP_FLOAT> state;
};

} // namespace dsp
