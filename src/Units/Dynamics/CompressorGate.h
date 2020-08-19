#pragma once

#include "AbsoluteValue.h"
#include "Base2Log.h"
#include "GainComputer.h"
#include "GainEnvelope.h"
#include "GainScale.h"

namespace dsp {

class CompressorGate : public Processor {

public:
    CompressorGate();
    ~CompressorGate();

    std::shared_ptr<InputParameter> getControlSignal() const;
    std::shared_ptr<InputParameter> getThreshold() const;
    std::shared_ptr<InputParameter> getCompressionRatio() const;
    std::shared_ptr<InputParameter> getGateRatio() const;
    std::shared_ptr<InputParameter> getKnee() const;
    std::shared_ptr<InputParameter> getAttack() const;
    std::shared_ptr<InputParameter> getRelease() const;
    std::shared_ptr<OutputParameter> getGainDelta() const;

protected:
    void connect() override;
    void disconnect() override;

private:
    const std::shared_ptr<AbsoluteValue> absoluteValue;
    const std::shared_ptr<Base2Log> base2Log;
    const std::shared_ptr<GainComputer> gainComputer;
    const std::shared_ptr<GainEnvelope> gainEnvelope;
    const std::shared_ptr<GainScale> gainScale;

    const std::shared_ptr<InputParameter> controlSignal;
    const std::shared_ptr<InputParameter> threshold;
    const std::shared_ptr<InputParameter> compressionRatio;
    const std::shared_ptr<InputParameter> gateRatio;
    const std::shared_ptr<InputParameter> knee;
    const std::shared_ptr<InputParameter> attack;
    const std::shared_ptr<InputParameter> release;

    const std::shared_ptr<OutputParameter> gainDelta;
};

} // namespace dsp
