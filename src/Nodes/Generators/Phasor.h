#pragma once

#include "../Core/Producer.h"

namespace dsp {

class Phasor : public Producer {

public:
    struct Mode {
        static const int MIN = 0;
        static const int MAX = 1;
        static const int WRAPPED = 0;
        static const int UNBOUNDED = 1;
    };

    Phasor();

    std::shared_ptr<Input> getFrequency() const;
    std::shared_ptr<Input> getMode() const;
    std::shared_ptr<Input> getReset() const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> frequency;
    const std::shared_ptr<Input> mode;
    const std::shared_ptr<Input> reset;
    Array phase;
};

} // namespace dsp
