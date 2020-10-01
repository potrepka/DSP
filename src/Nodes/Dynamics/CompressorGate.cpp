#include "CompressorGate.h"

dsp::CompressorGate::CompressorGate()
        : Transformer(Type::RATIO, Type::RATIO)
        , control(std::make_shared<Input>(Type::RATIO))
        , threshold(std::make_shared<Input>(Type::LOGARITHMIC))
        , compressionRatio(std::make_shared<Input>(Type::RATIO, Space::TIME, 1.0))
        , gateRatio(std::make_shared<Input>(Type::RATIO, Space::TIME, 1.0))
        , halfKnee(std::make_shared<Input>(Type::LOGARITHMIC))
        , attack(std::make_shared<Input>(Type::SECONDS))
        , release(std::make_shared<Input>(Type::SECONDS))
        , gainDelta(std::make_shared<Output>(Type::LOGARITHMIC)) {
    getInputs().push_back(control);
    getInputs().push_back(threshold);
    getInputs().push_back(compressionRatio);
    getInputs().push_back(gateRatio);
    getInputs().push_back(halfKnee);
    getInputs().push_back(attack);
    getInputs().push_back(release);
    getOutputs().push_back(gainDelta);
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getControl() const {
    return control;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getThreshold() const {
    return threshold;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getCompressionRatio() const {
    return compressionRatio;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getGateRatio() const {
    return gateRatio;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getHalfKnee() const {
    return halfKnee;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getAttack() const {
    return attack;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getRelease() const {
    return release;
}

std::shared_ptr<dsp::Output> dsp::CompressorGate::getGainDelta() const {
    return gainDelta;
}

void dsp::CompressorGate::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    gainState.resize(numChannels, 0.0);
}

void dsp::CompressorGate::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *controlChannel = getControl()->getWrapper().getChannelPointer(channel);
        Sample *thresholdChannel = getThreshold()->getWrapper().getChannelPointer(channel);
        Sample *compressionRatioChannel = getCompressionRatio()->getWrapper().getChannelPointer(channel);
        Sample *gateRatioChannel = getGateRatio()->getWrapper().getChannelPointer(channel);
        Sample *halfKneeChannel = getHalfKnee()->getWrapper().getChannelPointer(channel);
        Sample *attackChannel = getAttack()->getWrapper().getChannelPointer(channel);
        Sample *releaseChannel = getRelease()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample *gainDeltaChannel = getGainDelta()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample &input = inputChannel[sample];
            Sample &control = controlChannel[sample];
            Sample &threshold = thresholdChannel[sample];
            Sample &compressionRatio = compressionRatioChannel[sample];
            Sample &gateRatio = gateRatioChannel[sample];
            Sample &halfKnee = halfKneeChannel[sample];
            Sample &attack = attackChannel[sample];
            Sample &release = releaseChannel[sample];
            Sample &output = outputChannel[sample];
            Sample &gainDelta = gainDeltaChannel[sample];
            Sample gain = log2(abs(control));
            Sample gainDeltaStatic = getGainDelta(gain, threshold, compressionRatio, gateRatio, halfKnee);
            Sample decay = (abs(gainDeltaStatic) > abs(gainState[channel]) ? attack : release) * getSampleRate();
            gainState[channel] = gainDeltaStatic + pow(0.001, 1.0 / decay) * (gainState[channel] - gainDeltaStatic);
            gainDelta = gainState[channel];
            output = input * exp2(gainDelta);
        }
    }
}

dsp::Sample dsp::CompressorGate::getGainDelta(size_t channel, Sample gain) const {
    DSP_ASSERT(channel < getNumChannels());
    size_t lastSample = getNumSamples() - 1;
    return getGainDelta(gain,
                        getThreshold()->getWrapper().getSample(channel, lastSample),
                        getCompressionRatio()->getWrapper().getSample(channel, lastSample),
                        getGateRatio()->getWrapper().getSample(channel, lastSample),
                        getHalfKnee()->getWrapper().getSample(channel, lastSample));
}

dsp::Sample dsp::CompressorGate::getGainDelta(const Sample &gain,
                                              const Sample &threshold,
                                              const Sample &compressionRatio,
                                              const Sample &gateRatio,
                                              const Sample &halfKnee) {
    Sample gainMinusThreshold = gain - threshold;
    if (isinf(gainMinusThreshold)) {
        return 0.0;
    }
    Sample posHalfKnee = abs(halfKnee);
    if (gainMinusThreshold > -posHalfKnee && gainMinusThreshold < posHalfKnee) {
        if (compressionRatio == 0.0) {
            return std::numeric_limits<double>::infinity();
        }
        if (isinf(gateRatio)) {
            return -gateRatio;
        }
        Sample ratioDifference = 1.0 / compressionRatio - gateRatio;
        if (isinf(posHalfKnee) && ratioDifference != 0.0) {
            return ratioDifference * std::numeric_limits<double>::infinity();
        }
        Sample knee = 2.0 * posHalfKnee;
        Sample kneeTimesGR = knee * gateRatio;
        Sample t = gainMinusThreshold / knee + 0.5;
        return t * (t * posHalfKnee * ratioDifference + kneeTimesGR) - 0.5 * kneeTimesGR - gainMinusThreshold;
    } else {
        return gainMinusThreshold * (gainMinusThreshold > 0.0 ? 1.0 / compressionRatio - 1.0 : gateRatio - 1.0);
    }
}
