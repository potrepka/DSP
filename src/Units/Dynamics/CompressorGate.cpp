#include "CompressorGate.h"

dsp::CompressorGate::CompressorGate()
        : Processor(Type::BIPOLAR, Type::BIPOLAR)
        , channelMix(std::make_shared<ChannelMix>(Type::BIPOLAR))
        , absoluteValue(std::make_shared<AbsoluteValue>(Type::BIPOLAR))
        , base2Log(std::make_shared<Base2Log>())
        , gainComputer(std::make_shared<GainComputer>())
        , gainEnvelope(std::make_shared<GainEnvelope>())
        , gainScale(std::make_shared<GainScale>(Type::BIPOLAR))
        , CONTROL_SIGNAL(pushInput(channelMix->getInputSignal()))
        , LINK(pushInput(channelMix->getMixAmount()))
        , THRESHOLD(pushInput(gainComputer->getThreshold()))
        , COMPRESSION_RATIO(pushInput(gainComputer->getCompressionRatio()))
        , GATE_RATIO(pushInput(gainComputer->getGateRatio()))
        , KNEE(pushInput(gainComputer->getKnee()))
        , ATTACK(pushInput(gainEnvelope->getAttack()))
        , RELEASE(pushInput(gainEnvelope->getRelease()))
        , GAIN_DELTA(pushOutput(gainEnvelope->getOutputSignal())) {
    absoluteValue->getOutputSignal()->setType(Type::RATIO);

    setInputSignal(gainScale->getInputSignal());
    setOutputSignal(gainScale->getOutputSignal());

    pushUnit(channelMix);
    pushUnit(absoluteValue);
    pushUnit(base2Log);
    pushUnit(gainComputer);
    pushUnit(gainEnvelope);
    pushUnit(gainScale);

    connect();
}

dsp::CompressorGate::~CompressorGate() {
    disconnect();
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getControlSignal() const {
    return getInput(CONTROL_SIGNAL);
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getLink() const {
    return getInput(LINK);
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getThreshold() const {
    return getInput(THRESHOLD);
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getCompressionRatio() const {
    return getInput(COMPRESSION_RATIO);
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getGateRatio() const {
    return getInput(GATE_RATIO);
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getKnee() const {
    return getInput(KNEE);
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getAttack() const {
    return getInput(ATTACK);
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getRelease() const {
    return getInput(RELEASE);
}

std::shared_ptr<dsp::OutputParameter> dsp::CompressorGate::getGainDelta() const {
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
