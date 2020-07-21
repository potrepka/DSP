#pragma once

#include "Processor.h"

namespace dsp {

class Biquad : public Processor {

public:
    enum class Mode { LOW_PASS, HIGH_PASS, BAND_PASS, BAND_STOP, LOW_SHELF, HIGH_SHELF, PEAK, ALL_PASS };

    Biquad();

    Mode getMode() const;
    void setMode(Mode mode);
    std::shared_ptr<InputParameter> getFrequency() const;
    std::shared_ptr<InputParameter> getResonance() const;
    std::shared_ptr<InputParameter> getGain() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> frequency;
    const std::shared_ptr<InputParameter> resonance;
    const std::shared_ptr<InputParameter> gain;

    Mode mode;
    Array x1;
    Array x2;
    Array y1;
    Array y2;
#if DSP_USE_VC
    Vector a0;
    Vector a1;
    Vector a2;
    Vector b0;
    Vector b1;
    Vector b2;

    void calculateCoefficients(const Vector &frequency, const Vector &resonance, const Vector &gain);
#else
    Sample a0;
    Sample a1;
    Sample a2;
    Sample b0;
    Sample b1;
    Sample b2;

    void calculateCoefficients(const Sample &frequency, const Sample &resonance, const Sample &gain);
#endif
};

} // namespace dsp
