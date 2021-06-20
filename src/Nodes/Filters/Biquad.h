#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Biquad : public Transformer {

public:
    struct Mode {
        static const int MIN = 0;
        static const int MAX = 7;
        static const int LOW_PASS = 0;
        static const int HIGH_PASS = 1;
        static const int BAND_PASS = 2;
        static const int BAND_STOP = 3;
        static const int LOW_SHELF = 4;
        static const int HIGH_SHELF = 5;
        static const int PEAK = 6;
        static const int ALL_PASS = 7;
    };

    Biquad();

    std::shared_ptr<Input> getFrequency() const;
    std::shared_ptr<Input> getResonance() const;
    std::shared_ptr<Input> getAmplitude() const;
    std::shared_ptr<Input> getMode() const;

    void getMagnitudeAndPhaseResponse(size_t channel, Sample frequency, Sample &magnitude, Sample &phase);

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> frequency;
    const std::shared_ptr<Input> resonance;
    const std::shared_ptr<Input> amplitude;
    const std::shared_ptr<Input> mode;
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

    static void calculateCoefficients(const Sample sampleRate,
                                      const Sample oneOverSampleRate,
                                      const Sample &frequency,
                                      const Sample &resonance,
                                      const Sample &amplitude,
                                      const Sample &mode,
                                      Sample &a0,
                                      Sample &a1,
                                      Sample &a2,
                                      Sample &b0,
                                      Sample &b1,
                                      Sample &b2);
};

} // namespace dsp
