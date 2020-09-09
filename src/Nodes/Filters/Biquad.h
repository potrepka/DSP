#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Biquad : public Transformer {

public:
    enum class Mode { LOW_PASS, HIGH_PASS, BAND_PASS, BAND_STOP, LOW_SHELF, HIGH_SHELF, PEAK, ALL_PASS };

    Biquad();

    Mode getMode() const;
    void setMode(Mode mode);

    std::shared_ptr<Input> getResetTrigger() const;
    std::shared_ptr<Input> getFrequency() const;
    std::shared_ptr<Input> getResonance() const;
    std::shared_ptr<Input> getGain() const;

protected:
    void setNumOutputChannelsNoLock(int numChannels) override;
    void processNoLock() override;

private:
    Mode mode;
    const std::shared_ptr<Input> resetTrigger;
    const std::shared_ptr<Input> frequency;
    const std::shared_ptr<Input> resonance;
    const std::shared_ptr<Input> gain;
    Array x1;
    Array x2;
    Array y1;
    Array y2;
    Sample a0;
    Sample a1;
    Sample a2;
    Sample b0;
    Sample b1;
    Sample b2;

    void calculateCoefficients(const Sample &frequency, const Sample &resonance, const Sample &gain);
};

} // namespace dsp
