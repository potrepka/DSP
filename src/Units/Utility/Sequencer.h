#pragma once

#include "Buffer.h"
#include "Generator.h"

namespace dsp {

class Sequencer : public Generator {

public:
    Sequencer(Type type);
    std::shared_ptr<Buffer> getSequence();
    void setSequence(std::shared_ptr<Buffer> sequence);
    std::shared_ptr<InputParameter> getResetTrigger();
    std::shared_ptr<InputParameter> getTrigger();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const unsigned int RESET_TRIGGER;
    static const unsigned int TRIGGER;
    std::shared_ptr<Buffer> sequence;
    std::vector<unsigned int> memory;
    std::vector<unsigned int> index;
};

} // namespace dsp
