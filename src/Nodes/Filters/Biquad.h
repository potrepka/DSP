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

    void getMagnitudeAndPhaseResponse(size_t channel, Sample frequency, Sample &magnitude, Sample &phase) const;

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    Mode mode;
    const std::shared_ptr<Input> resetTrigger;
    const std::shared_ptr<Input> frequency;
    const std::shared_ptr<Input> resonance;
    const std::shared_ptr<Input> gain;
    Array xx1;
    Array xx2;
    Array yy1;
    Array yy2;
    Array aa0;
    Array aa1;
    Array aa2;
    Array bb0;
    Array bb1;
    Array bb2;

    void calculateCoefficients(const Sample &frequency,
                               const Sample &resonance,
                               const Sample &gain,
                               Sample &a0,
                               Sample &a1,
                               Sample &a2,
                               Sample &b0,
                               Sample &b1,
                               Sample &b2) const;
};

} // namespace dsp
