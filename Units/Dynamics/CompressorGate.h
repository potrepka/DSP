#pragma once

#include "ChannelMix.h"
#include "AbsoluteValue.h"
#include "ToDecibels.h"
#include "GainComputer.h"
#include "Envelope.h"
#include "GainUnit.h"

namespace dsp {

class CompressorGate : public Filter {

public:
    CompressorGate();
    void setNumChannels(std::size_t size) override;
    std::shared_ptr<InputParameter> getLink();
    std::shared_ptr<InputParameter> getThreshold();
    std::shared_ptr<InputParameter> getCompressionRatio();
    std::shared_ptr<InputParameter> getGateRatio();
    std::shared_ptr<InputParameter> getKnee();
    std::shared_ptr<InputParameter> getAttack();
    std::shared_ptr<InputParameter> getRelease();

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

    static const std::size_t CHANNEL_MIX;
    static const std::size_t ABSOLUTE_VALUE;
    static const std::size_t TO_DECIBELS;
    static const std::size_t GAIN_COMPUTER;
    static const std::size_t ENVELOPE;
    static const std::size_t GAIN_UNIT;

};

}
