#pragma once

#include "Processor.h"

namespace dsp {

class DryWetMix : public Processor {

public:
    DryWetMix(Connection::Type type, Connection::Space space = Connection::Space::TIME);
    std::shared_ptr<InputParameter> getDrySignal();
    std::shared_ptr<InputParameter> getWetSignal();
    std::shared_ptr<InputParameter> getMix();

protected:
    void process() override;

private:
    static const std::size_t DRY_SIGNAL;
    static const std::size_t WET_SIGNAL;
    static const std::size_t MIX;
};

} // namespace dsp
