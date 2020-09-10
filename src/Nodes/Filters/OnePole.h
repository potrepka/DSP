#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class OnePole : public Transformer {

public:
    enum class Mode { LOW_PASS, HIGH_PASS };

    OnePole(Type type);

    Mode getMode() const;
    void setMode(Mode mode);

    std::shared_ptr<Input> getFrequency() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    Mode mode;
    const std::shared_ptr<Input> frequency;
    Array state;
};

} // namespace dsp
