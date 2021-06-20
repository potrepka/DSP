#pragma once

#include "../Core/Producer.h"

namespace dsp {

class OnOff : public Producer {

public:
    OnOff();

    std::shared_ptr<Input> getOnTrigger() const;
    std::shared_ptr<Input> getOffTrigger() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> onTrigger;
    const std::shared_ptr<Input> offTrigger;
    std::vector<int> state;
};

} // namespace dsp
