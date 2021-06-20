#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class OnePole : public Transformer {

public:
    struct Mode {
        static const int MIN = 0;
        static const int MAX = 1;
        static const int LOW_PASS = 0;
        static const int HIGH_PASS = 1;
    };

    OnePole(Type type = Type::RATIO);

    std::shared_ptr<Input> getFrequency() const;
    std::shared_ptr<Input> getMode() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> frequency;
    const std::shared_ptr<Input> mode;
    Array state;
};

} // namespace dsp
