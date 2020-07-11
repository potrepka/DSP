#pragma once

#include "AbsoluteValue.h"
#include "Base2Log.h"
#include "ChannelMix.h"
#include "GainComputer.h"
#include "GainEnvelope.h"
#include "GainUnit.h"

namespace dsp {

class CompressorGate : public Processor {

public:
    CompressorGate();
    ~CompressorGate();
    std::shared_ptr<InputParameter> getLink() const;
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
    static const unsigned int CONTROL_SIGNAL;
    static const unsigned int LINK;
    static const unsigned int THRESHOLD;
    static const unsigned int COMPRESSION_RATIO;
    static const unsigned int GATE_RATIO;
    static const unsigned int KNEE;
    static const unsigned int ATTACK;
    static const unsigned int RELEASE;

    static const unsigned int GAIN_DELTA;

    std::shared_ptr<ChannelMix> channelMix;
    std::shared_ptr<AbsoluteValue> absoluteValue;
    std::shared_ptr<Base2Log> ratioToDecibels;
    std::shared_ptr<GainComputer> gainComputer;
    std::shared_ptr<GainEnvelope> gainEnvelope;
    std::shared_ptr<GainUnit> gainUnit;
};

} // namespace dsp
