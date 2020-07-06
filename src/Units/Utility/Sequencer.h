#pragma once

#include "Generator.h"

namespace dsp {

class Sequencer : public Generator {

public:
    Sequencer();
    std::vector<unsigned int> &getSequence();
    void setSequence(std::vector<unsigned int> &sequence);
    void removeSequence();
    std::shared_ptr<InputParameter> getResetTrigger();
    std::shared_ptr<InputParameter> getTrigger();

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    static const unsigned int RESET_TRIGGER;
    static const unsigned int TRIGGER;
    std::vector<unsigned int> *sequence;
    std::vector<unsigned int> memory;
    std::vector<unsigned int> index;
};

} // namespace dsp
