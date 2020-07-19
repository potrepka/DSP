#pragma once

#include "Processor.h"

namespace dsp {

class OnePole : public Processor {

public:
    enum class Mode { LOW_PASS, HIGH_PASS };

    OnePole(Type type);

    Mode getMode() const;
    void setMode(Mode mode);
    std::shared_ptr<InputParameter> getFrequency() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> frequency;

    Mode mode;
    std::vector<Sample> state;
};

} // namespace dsp
