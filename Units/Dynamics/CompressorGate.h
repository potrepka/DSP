#pragma once

#include "AbsoluteValue.h"
#include "ChannelMix.h"
#include "Envelope.h"
#include "GainComputer.h"
#include "GainUnit.h"
#include "ToDecibels.h"

namespace dsp {

class CompressorGate : public Processor {

public:
    CompressorGate();
    ~CompressorGate();
    std::shared_ptr<InputParameter> getLink();
    std::shared_ptr<InputParameter> getThreshold();
    std::shared_ptr<InputParameter> getCompressionRatio();
    std::shared_ptr<InputParameter> getGateRatio();
    std::shared_ptr<InputParameter> getKnee();
    std::shared_ptr<InputParameter> getAttack();
    std::shared_ptr<InputParameter> getRelease();
    std::shared_ptr<OutputParameter> getGainDelta();

protected:
    void connect() override;
    void disconnect() override;

private:
    static const std::size_t CONTROL_SIGNAL;
    static const std::size_t LINK;
    static const std::size_t THRESHOLD;
    static const std::size_t COMPRESSION_RATIO;
    static const std::size_t GATE_RATIO;
    static const std::size_t KNEE;
    static const std::size_t ATTACK;
    static const std::size_t RELEASE;

    static const std::size_t GAIN_DELTA;

    static const std::size_t CHANNEL_MIX;
    static const std::size_t ABSOLUTE_VALUE;
    static const std::size_t TO_DECIBELS;
    static const std::size_t GAIN_COMPUTER;
    static const std::size_t ENVELOPE;
    static const std::size_t GAIN_UNIT;

    std::shared_ptr<ChannelMix> channelMix;
    std::shared_ptr<AbsoluteValue> absoluteValue;
    std::shared_ptr<ToDecibels> decibels;
    std::shared_ptr<GainComputer> gainComputer;
    std::shared_ptr<Envelope> envelope;
    std::shared_ptr<GainUnit> gainUnit;
};

} // namespace dsp
