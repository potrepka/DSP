#pragma once

#include "../Core/Producer.h"

namespace dsp {

class ResetTrigger : public Producer {

public:
    ResetTrigger();

    void reset();
    void reset(size_t channel);

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    std::vector<int> state;
};

} // namespace dsp
