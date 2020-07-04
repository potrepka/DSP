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

dsp::CompressorGate::CompressorGate() : Processor(Connection::Type::BIPOLAR, Connection::Type::BIPOLAR) {
    channelMix = std::make_shared<ChannelMix>(Connection::Type::BIPOLAR);
    absoluteValue = std::make_shared<AbsoluteValue>(Connection::Type::RATIO);
    ratioToDecibels = std::make_shared<Base2Log>();
    gainComputer = std::make_shared<GainComputer>();
    gainEnvelope = std::make_shared<GainEnvelope>();
    gainUnit = std::make_shared<GainUnit>();

    absoluteValue->getInputSignal()->setType(Connection::Type::BIPOLAR);

    pushUnit(channelMix);
    pushUnit(absoluteValue);
    pushUnit(ratioToDecibels);
    pushUnit(gainComputer);
    pushUnit(gainEnvelope);
    pushUnit(gainUnit);

    setInputSignal(gainUnit->getInputSignal());
    pushInput(channelMix->getInputSignal());
    pushInput(channelMix->getMix());
    pushInput(gainComputer->getThreshold());
    pushInput(gainComputer->getCompressionRatio());
    pushInput(gainComputer->getGateRatio());
    pushInput(gainComputer->getKnee());
    pushInput(gainEnvelope->getAttack());
    pushInput(gainEnvelope->getRelease());

    setOutputSignal(gainUnit->getOutputSignal());
    pushOutput(gainEnvelope->getOutputSignal());

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
    absoluteValue->getOutputSignal() >> ratioToDecibels->getInputSignal();
    ratioToDecibels->getOutputSignal() >> gainComputer->getInputSignal();
    gainComputer->getOutputSignal() >> gainEnvelope->getInputSignal();
    gainEnvelope->getOutputSignal() >> gainUnit->getGain();
}

void dsp::CompressorGate::disconnect() {
    channelMix->getOutputSignal() != absoluteValue->getInputSignal();
    absoluteValue->getOutputSignal() != ratioToDecibels->getInputSignal();
    ratioToDecibels->getOutputSignal() != gainComputer->getInputSignal();
    gainComputer->getOutputSignal() != gainEnvelope->getInputSignal();
    gainEnvelope->getOutputSignal() != gainUnit->getGain();
}
