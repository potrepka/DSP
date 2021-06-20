#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class CompressorGate : public Transformer {

public:
    CompressorGate();

    std::shared_ptr<Input> getControl() const;
    std::shared_ptr<Input> getThreshold() const;
    std::shared_ptr<Input> getSoftness() const;
    std::shared_ptr<Input> getCompressionRatio() const;
    std::shared_ptr<Input> getGateRatio() const;
    std::shared_ptr<Input> getAttack() const;
    std::shared_ptr<Input> getRelease() const;
    std::shared_ptr<Output> getGain() const;

    Sample getGainResponse(size_t channel, Sample gain);

protected:
    void setNumOutputChannelsNoLock(size_t numChannels) override;
    void processNoLock() override;

private:
    const std::shared_ptr<Input> control;
    const std::shared_ptr<Input> threshold;
    const std::shared_ptr<Input> softness;
    const std::shared_ptr<Input> compressionRatio;
    const std::shared_ptr<Input> gateRatio;
    const std::shared_ptr<Input> attack;
    const std::shared_ptr<Input> release;
    const std::shared_ptr<Output> gain;
    Array state;

    static Sample getGainResponse(const Sample &gain,
                                  const Sample &threshold,
                                  const Sample &softness,
                                  const Sample &compressionRatio,
                                  const Sample &gateRatio);
};

} // namespace dsp
