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
    static const unsigned int FREQUENCY;
    static const unsigned int Q;
    static const unsigned int GAIN;

    Mode mode;
    std::vector<DSP_FLOAT> x1;
    std::vector<DSP_FLOAT> x2;
    std::vector<DSP_FLOAT> y1;
    std::vector<DSP_FLOAT> y2;
    DSP_FLOAT a0;
    DSP_FLOAT a1;
    DSP_FLOAT a2;
    DSP_FLOAT b0;
    DSP_FLOAT b1;
    DSP_FLOAT b2;

    void calculateCoefficients(const DSP_FLOAT &frequency, const DSP_FLOAT &q, const DSP_FLOAT &gain);
};

} // namespace dsp
