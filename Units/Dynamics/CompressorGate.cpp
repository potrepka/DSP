#include "CompressorGate.h"

const std::size_t dsp::CompressorGate::CONTROL_SIGNAL = 1;
const std::size_t dsp::CompressorGate::LINK = 2;
const std::size_t dsp::CompressorGate::THRESHOLD = 3;
const std::size_t dsp::CompressorGate::COMPRESSION_RATIO = 4;
const std::size_t dsp::CompressorGate::GATE_RATIO = 5;
const std::size_t dsp::CompressorGate::KNEE = 6;
const std::size_t dsp::CompressorGate::ATTACK = 7;
const std::size_t dsp::CompressorGate::RELEASE = 8;

const std::size_t dsp::CompressorGate::GAIN_DELTA = 1;

const std::size_t dsp::CompressorGate::CHANNEL_MIX = 0;
const std::size_t dsp::CompressorGate::ABSOLUTE_VALUE = 1;
const std::size_t dsp::CompressorGate::TO_DECIBELS = 2;
const std::size_t dsp::CompressorGate::GAIN_COMPUTER = 3;
const std::size_t dsp::CompressorGate::ENVELOPE = 4;
const std::size_t dsp::CompressorGate::GAIN_UNIT = 5;

dsp::CompressorGate::CompressorGate() : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR) {
    channelMix = std::make_shared<ChannelMix>();
    absoluteValue = std::make_shared<AbsoluteValue>();
    decibels = std::make_shared<ToDecibels>();
    gainComputer = std::make_shared<GainComputer>();
    envelope = std::make_shared<Envelope>();
    envelope->getInputSignal()->setType(Connection::Type::DECIBELS);
    envelope->getOutputSignal()->setType(Connection::Type::DECIBELS);
    gainUnit = std::make_shared<GainUnit>();

    pushUnit(channelMix);
    pushUnit(absoluteValue);
    pushUnit(decibels);
    pushUnit(gainComputer);
    pushUnit(envelope);
    pushUnit(gainUnit);

    setInputSignal(gainUnit->getInputSignal());
    pushInput(channelMix->getInputSignal());
    pushInput(channelMix->getMix());
    pushInput(gainComputer->getThreshold());
    pushInput(gainComputer->getCompressionRatio());
    pushInput(gainComputer->getGateRatio());
    pushInput(gainComputer->getKnee());
    pushInput(envelope->getAttack());
    pushInput(envelope->getRelease());

    setOutputSignal(gainUnit->getOutputSignal());
    pushOutput(envelope->getOutputSignal());

    connect();
}

dsp::CompressorGate::~CompressorGate() {
    disconnect();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getLink() {
    return getInput(LINK);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getThreshold() {
    return getInput(THRESHOLD);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getCompressionRatio() {
    return getInput(COMPRESSION_RATIO);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getGateRatio() {
    return getInput(GATE_RATIO);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getKnee() {
    return getInput(KNEE);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getAttack() {
    return getInput(ATTACK);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getRelease() {
    return getInput(RELEASE);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::CompressorGate::getGainDelta() {
    return getOutput(GAIN_DELTA);
}

void dsp::CompressorGate::connect() {
    channelMix->getOutputSignal() >> absoluteValue->getInputSignal();
    absoluteValue->getOutputSignal() >> decibels->getInputSignal();
    decibels->getOutputSignal() >> gainComputer->getInputSignal();
    gainComputer->getOutputSignal() >> envelope->getInputSignal();
    envelope->getOutputSignal() >> gainUnit->getGain();
}

void dsp::CompressorGate::disconnect() {
    channelMix->getOutputSignal() != absoluteValue->getInputSignal();
    absoluteValue->getOutputSignal() != decibels->getInputSignal();
    decibels->getOutputSignal() != gainComputer->getInputSignal();
    gainComputer->getOutputSignal() != envelope->getInputSignal();
    envelope->getOutputSignal() != gainUnit->getGain();
}
