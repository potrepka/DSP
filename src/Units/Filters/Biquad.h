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
    std::shared_ptr<InputParameter> getQ() const;
    std::shared_ptr<InputParameter> getGain() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> frequency;
    const std::shared_ptr<InputParameter> q;
    const std::shared_ptr<InputParameter> gain;

    Mode mode;
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

    void calculateCoefficients(const Sample &frequency, const Sample &q, const Sample &gain);
};

} // namespace dsp
