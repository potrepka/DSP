#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class FrequencyToNote : public Transformer {

public:
    FrequencyToNote(Space space = Space::TIME);

    std::shared_ptr<Input> getTuningFrequency() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> tuningFrequency;
};

} // namespace dsp
