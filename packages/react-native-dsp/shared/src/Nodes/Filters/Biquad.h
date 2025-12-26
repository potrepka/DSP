#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class Biquad : public Transformer {
public:
  struct Mode {
    static constexpr int MIN = 0;
    static constexpr int MAX = 7;
    static constexpr int LOW_PASS = 0;
    static constexpr int HIGH_PASS = 1;
    static constexpr int BAND_PASS = 2;
    static constexpr int BAND_STOP = 3;
    static constexpr int LOW_SHELF = 4;
    static constexpr int HIGH_SHELF = 5;
    static constexpr int PEAK = 6;
    static constexpr int ALL_PASS = 7;
  };

  Biquad();

  std::shared_ptr<Input> getFrequency() const;
  std::shared_ptr<Input> getResonance() const;
  std::shared_ptr<Input> getAmplitude() const;
  std::shared_ptr<Input> getMode() const;

  FrequencyResponse getFrequencyResponse(size_t channel, Sample frequency);

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
                                    const Sample& frequency,
                                    const Sample& resonance,
                                    const Sample& amplitude, const Sample& mode,
                                    Sample& a0, Sample& a1, Sample& a2,
                                    Sample& b0, Sample& b1, Sample& b2);
};

} // namespace dsp
