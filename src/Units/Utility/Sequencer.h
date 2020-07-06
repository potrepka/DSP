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
    void setNumChannelsNoLock(std::size_t numChannels) override;
    void process() override;

private:
    static const std::size_t RESET_TRIGGER;
    static const std::size_t TRIGGER;
    std::vector<unsigned int> *sequence;
    std::vector<std::size_t> memory;
    std::vector<std::size_t> index;
};

} // namespace dsp
