#include "CompressorGate.h"

const std::size_t dsp::CompressorGate::CONTROL_SIGNAL = 1;
const std::size_t dsp::CompressorGate::LINK = 2;
const std::size_t dsp::CompressorGate::THRESHOLD = 3;
const std::size_t dsp::CompressorGate::COMPRESSION_RATIO = 4;
const std::size_t dsp::CompressorGate::GATE_RATIO = 5;
const std::size_t dsp::CompressorGate::KNEE = 6;
const std::size_t dsp::CompressorGate::ATTACK = 7;
const std::size_t dsp::CompressorGate::RELEASE = 8;

const std::size_t dsp::CompressorGate::CHANNEL_MIX = 0;
const std::size_t dsp::CompressorGate::ABSOLUTE_VALUE = 1;
const std::size_t dsp::CompressorGate::TO_DECIBELS = 2;
const std::size_t dsp::CompressorGate::GAIN_COMPUTER = 3;
const std::size_t dsp::CompressorGate::ENVELOPE = 4;
const std::size_t dsp::CompressorGate::GAIN_UNIT = 5;

dsp::CompressorGate::CompressorGate() : Filter(Connection::Type::BIPOLAR) {
    ChannelMix *channelMix = new ChannelMix();
    AbsoluteValue *absoluteValue = new AbsoluteValue();
    ToDecibels *decibels = new ToDecibels();
    GainComputer *gainComputer = new GainComputer();
    Envelope *envelope = new Envelope();
    envelope->getInputSignal()->setType(Connection::Type::DECIBELS);
    envelope->getOutputSignal()->setType(Connection::Type::DECIBELS);
    GainUnit *gainUnit = new GainUnit();

    pushUnit(channelMix);
    pushUnit(absoluteValue);
    pushUnit(decibels);
    pushUnit(gainComputer);
    pushUnit(envelope);
    pushUnit(gainUnit);

    inputs[0] = gainUnit->getInputSignal();
    outputs[0] = gainUnit->getOutputSignal();
    pushInput(channelMix->getInputSignal());
    pushInput(channelMix->getMix());
    pushInput(gainComputer->getThreshold());
    pushInput(gainComputer->getCompressionRatio());
    pushInput(gainComputer->getGateRatio());
    pushInput(gainComputer->getKnee());
    pushInput(envelope->getAttack());
    pushInput(envelope->getRelease());

    connect();
}

void dsp::CompressorGate::setNumChannels(std::size_t size) {
    lock();
    disconnect();
    dynamic_cast<ChannelMix *>(getUnit(CHANNEL_MIX).get())->setNumChannels(size);
    dynamic_cast<AbsoluteValue *>(getUnit(ABSOLUTE_VALUE).get())->setNumChannels(size);
    dynamic_cast<ToDecibels *>(getUnit(TO_DECIBELS).get())->setNumChannels(size);
    dynamic_cast<GainComputer *>(getUnit(GAIN_COMPUTER).get())->setNumChannels(size);
    dynamic_cast<Envelope *>(getUnit(ENVELOPE).get())->setNumChannels(size);
    dynamic_cast<GainUnit *>(getUnit(GAIN_UNIT).get())->setNumChannels(size);
    connect();
    unlock();
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

void dsp::CompressorGate::disconnect() {
    ChannelMix *channelMix = dynamic_cast<ChannelMix *>(getUnit(CHANNEL_MIX).get());
    AbsoluteValue *absoluteValue = dynamic_cast<AbsoluteValue *>(getUnit(ABSOLUTE_VALUE).get());
    ToDecibels *decibels = dynamic_cast<ToDecibels *>(getUnit(TO_DECIBELS).get());
    GainComputer *gainComputer = dynamic_cast<GainComputer *>(getUnit(GAIN_COMPUTER).get());
    Envelope *envelope = dynamic_cast<Envelope *>(getUnit(ENVELOPE).get());
    GainUnit *gainUnit = dynamic_cast<GainUnit *>(getUnit(GAIN_UNIT).get());

    *channelMix->getOutputSignal() != *absoluteValue->getInputSignal();
    *absoluteValue->getOutputSignal() != *decibels->getInputSignal();
    *decibels->getOutputSignal() != *gainComputer->getInputSignal();
    *gainComputer->getOutputSignal() != *envelope->getInputSignal();
    *envelope->getOutputSignal() != *gainUnit->getGain();
}

void dsp::CompressorGate::connect() {
    ChannelMix *channelMix = dynamic_cast<ChannelMix *>(getUnit(CHANNEL_MIX).get());
    AbsoluteValue *absoluteValue = dynamic_cast<AbsoluteValue *>(getUnit(ABSOLUTE_VALUE).get());
    ToDecibels *decibels = dynamic_cast<ToDecibels *>(getUnit(TO_DECIBELS).get());
    GainComputer *gainComputer = dynamic_cast<GainComputer *>(getUnit(GAIN_COMPUTER).get());
    Envelope *envelope = dynamic_cast<Envelope *>(getUnit(ENVELOPE).get());
    GainUnit *gainUnit = dynamic_cast<GainUnit *>(getUnit(GAIN_UNIT).get());

    *channelMix->getOutputSignal() >> *absoluteValue->getInputSignal();
    *absoluteValue->getOutputSignal() >> *decibels->getInputSignal();
    *decibels->getOutputSignal() >> *gainComputer->getInputSignal();
    *gainComputer->getOutputSignal() >> *envelope->getInputSignal();
    *envelope->getOutputSignal() >> *gainUnit->getGain();
}
