#include "CompressorGate.h"

const unsigned int dsp::CompressorGate::CONTROL_SIGNAL = 1;
const unsigned int dsp::CompressorGate::LINK = 2;
const unsigned int dsp::CompressorGate::THRESHOLD = 3;
const unsigned int dsp::CompressorGate::COMPRESSION_RATIO = 4;
const unsigned int dsp::CompressorGate::GATE_RATIO = 5;
const unsigned int dsp::CompressorGate::KNEE = 6;
const unsigned int dsp::CompressorGate::ATTACK = 7;
const unsigned int dsp::CompressorGate::RELEASE = 8;

const unsigned int dsp::CompressorGate::GAIN_DELTA = 1;

dsp::CompressorGate::CompressorGate() : Processor(Type::BIPOLAR, Type::BIPOLAR) {
    channelMix = std::make_shared<ChannelMix>(Type::BIPOLAR);
    absoluteValue = std::make_shared<AbsoluteValue>(Type::BIPOLAR);
    base2Log = std::make_shared<Base2Log>();
    gainComputer = std::make_shared<GainComputer>();
    gainEnvelope = std::make_shared<GainEnvelope>();
    gainScale = std::make_shared<GainScale>(Type::BIPOLAR);

    absoluteValue->getOutputSignal()->setType(Type::RATIO);

    pushUnit(channelMix);
    pushUnit(absoluteValue);
    pushUnit(base2Log);
    pushUnit(gainComputer);
    pushUnit(gainEnvelope);
    pushUnit(gainScale);

    setInputSignal(gainScale->getInputSignal());
    pushInput(channelMix->getInputSignal());
    pushInput(channelMix->getMixAmount());
    pushInput(gainComputer->getThreshold());
    pushInput(gainComputer->getCompressionRatio());
    pushInput(gainComputer->getGateRatio());
    pushInput(gainComputer->getKnee());
    pushInput(gainEnvelope->getAttack());
    pushInput(gainEnvelope->getRelease());

    setOutputSignal(gainScale->getOutputSignal());
    pushOutput(gainEnvelope->getOutputSignal());

    connect();
}

dsp::CompressorGate::~CompressorGate() {
    disconnect();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getLink() const {
    return getInput(LINK);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getThreshold() const {
    return getInput(THRESHOLD);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getCompressionRatio() const {
    return getInput(COMPRESSION_RATIO);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getGateRatio() const {
    return getInput(GATE_RATIO);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getKnee() const {
    return getInput(KNEE);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getAttack() const {
    return getInput(ATTACK);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::CompressorGate::getRelease() const {
    return getInput(RELEASE);
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::CompressorGate::getGainDelta() const {
    return getOutput(GAIN_DELTA);
}

void dsp::CompressorGate::connect() {
    channelMix->getOutputSignal() >> absoluteValue->getInputSignal();
    absoluteValue->getOutputSignal() >> base2Log->getInputSignal();
    base2Log->getOutputSignal() >> gainComputer->getInputSignal();
    gainComputer->getOutputSignal() >> gainEnvelope->getInputSignal();
    gainEnvelope->getOutputSignal() >> gainScale->getGain();
}

void dsp::CompressorGate::disconnect() {
    channelMix->getOutputSignal() != absoluteValue->getInputSignal();
    absoluteValue->getOutputSignal() != base2Log->getInputSignal();
    base2Log->getOutputSignal() != gainComputer->getInputSignal();
    gainComputer->getOutputSignal() != gainEnvelope->getInputSignal();
    gainEnvelope->getOutputSignal() != gainScale->getGain();
}
