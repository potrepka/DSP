#pragma once

#include "Buffer.h"
#include "Generator.h"

namespace dsp {

class Sequencer : public Generator {

public:
    Sequencer(Type type);

    std::shared_ptr<Buffer> getSequence() const;
    void setSequence(std::shared_ptr<Buffer> sequence);
    std::shared_ptr<InputParameter> getResetTrigger() const;
    std::shared_ptr<InputParameter> getTrigger() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const unsigned int RESET_TRIGGER;
    const unsigned int TRIGGER;

    std::shared_ptr<Buffer> sequence;
    std::vector<unsigned int> memory;
    std::vector<unsigned int> index;
};

} // namespace dsp
