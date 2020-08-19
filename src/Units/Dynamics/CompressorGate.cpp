#include "CompressorGate.h"

dsp::CompressorGate::CompressorGate()
        : Processor(Type::BIPOLAR, Type::BIPOLAR)
        , absoluteValue(std::make_shared<AbsoluteValue>(Type::BIPOLAR))
        , base2Log(std::make_shared<Base2Log>())
        , gainComputer(std::make_shared<GainComputer>())
        , gainEnvelope(std::make_shared<GainEnvelope>())
        , gainScale(std::make_shared<GainScale>(Type::BIPOLAR))
        , controlSignal(absoluteValue->getInputSignal())
        , threshold(gainComputer->getThreshold())
        , compressionRatio(gainComputer->getCompressionRatio())
        , gateRatio(gainComputer->getGateRatio())
        , knee(gainComputer->getKnee())
        , attack(gainEnvelope->getAttack())
        , release(gainEnvelope->getRelease())
        , gainDelta(gainEnvelope->getOutputSignal()) {
    absoluteValue->getOutputSignal()->setType(Type::RATIO);

    setInputSignal(gainScale->getInputSignal());
    pushInput(controlSignal);
    pushInput(threshold);
    pushInput(compressionRatio);
    pushInput(gateRatio);
    pushInput(knee);
    pushInput(attack);
    pushInput(release);

    setOutputSignal(gainScale->getOutputSignal());
    pushOutput(gainDelta);

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
    return controlSignal;
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getThreshold() const {
    return threshold;
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getCompressionRatio() const {
    return compressionRatio;
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getGateRatio() const {
    return gateRatio;
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getKnee() const {
    return knee;
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getAttack() const {
    return attack;
}

std::shared_ptr<dsp::InputParameter> dsp::CompressorGate::getRelease() const {
    return release;
}

std::shared_ptr<dsp::OutputParameter> dsp::CompressorGate::getGainDelta() const {
    return gainDelta;
}

void dsp::CompressorGate::connect() {
    absoluteValue->getOutputSignal() >> base2Log->getInputSignal();
    base2Log->getOutputSignal() >> gainComputer->getInputSignal();
    gainComputer->getOutputSignal() >> gainEnvelope->getInputSignal();
    gainEnvelope->getOutputSignal() >> gainScale->getGain();
}

void dsp::CompressorGate::disconnect() {
    absoluteValue->getOutputSignal() != base2Log->getInputSignal();
    base2Log->getOutputSignal() != gainComputer->getInputSignal();
    gainComputer->getOutputSignal() != gainEnvelope->getInputSignal();
    gainEnvelope->getOutputSignal() != gainScale->getGain();
}
