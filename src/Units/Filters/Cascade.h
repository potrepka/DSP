#pragma once

#include "Biquad.h"
#include "Multiply.h"
#include "OnePole.h"
#include "PassThrough.h"

namespace dsp {

class Cascade : public Processor {

public:
    enum class Mode { LOW_PASS, HIGH_PASS, BAND_PASS, BAND_STOP, LOW_SHELF, HIGH_SHELF, PEAK, ALL_PASS };

    Cascade();
    ~Cascade();

    Mode getMode() const;
    void setMode(Mode mode);
    unsigned int getOrder() const;
    void setOrder(unsigned int order);
    std::shared_ptr<InputParameter> getFrequency() const;
    std::shared_ptr<InputParameter> getResonance() const;
    std::shared_ptr<InputParameter> getGain() const;

protected:
    void connect() override;
    void disconnect() override;

private:
    const std::shared_ptr<PassThrough> input;
    const std::shared_ptr<PassThrough> frequency;
    const std::shared_ptr<PassThrough> resonance;
    const std::shared_ptr<PassThrough> gain;
    const std::shared_ptr<OnePole> onePole;
    const std::shared_ptr<PassThrough> output;

    Mode mode;
    unsigned int halfOrder;
    std::vector<std::shared_ptr<Multiply>> resonanceFactors;
    std::vector<std::shared_ptr<Multiply>> gainFactors;
    std::vector<std::shared_ptr<Biquad>> biquads;

    void setMode(std::shared_ptr<Biquad> biquad);
};

} // namespace dsp
