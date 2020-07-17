#pragma once

#include "Generator.h"

namespace dsp {

class OnOff : public Generator {

public:
    OnOff();

    std::shared_ptr<InputParameter> getOnTrigger() const;
    std::shared_ptr<InputParameter> getOffTrigger() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> onTrigger;
    const std::shared_ptr<InputParameter> offTrigger;

    std::vector<unsigned int> state;
};

} // namespace dsp
