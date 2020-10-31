#include "CompressorGate.h"

dsp::CompressorGate::CompressorGate()
        : Transformer(Type::RATIO, Type::RATIO)
        , control(std::make_shared<Input>(Type::RATIO))
        , threshold(std::make_shared<Input>(Type::LOGARITHMIC))
        , halfKnee(std::make_shared<Input>(Type::LOGARITHMIC))
        , compressionRatio(std::make_shared<Input>(Type::RATIO, Space::TIME, 1.0))
        , gateRatio(std::make_shared<Input>(Type::RATIO, Space::TIME, 1.0))
        , attack(std::make_shared<Input>(Type::SECONDS))
        , release(std::make_shared<Input>(Type::SECONDS))
        , gainResponse(std::make_shared<Output>(Type::LOGARITHMIC)) {
    getInputs().push_back(control);
    getInputs().push_back(threshold);
    getInputs().push_back(halfKnee);
    getInputs().push_back(compressionRatio);
    getInputs().push_back(gateRatio);
    getInputs().push_back(attack);
    getInputs().push_back(release);
    getOutputs().push_back(gainResponse);
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getControl() const {
    return control;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getThreshold() const {
    return threshold;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getHalfKnee() const {
    return halfKnee;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getCompressionRatio() const {
    return compressionRatio;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getGateRatio() const {
    return gateRatio;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getAttack() const {
    return attack;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getRelease() const {
    return release;
}

std::shared_ptr<dsp::Output> dsp::CompressorGate::getGainResponse() const {
    return gainResponse;
}

dsp::Sample dsp::CompressorGate::getGainResponse(size_t channel, Sample gain) {
    lock();
    DSP_ASSERT(channel < getNumChannels());
    if (getNumSamples() > 0) {
        const Sample threshold = getThreshold()->getWrapper().getSample(channel, 0);
        const Sample compressionRatio = getCompressionRatio()->getWrapper().getSample(channel, 0);
        const Sample gateRatio = getGateRatio()->getWrapper().getSample(channel, 0);
        const Sample halfKnee = getHalfKnee()->getWrapper().getSample(channel, 0);
        unlock();
        return getGainResponse(gain, threshold, compressionRatio, gateRatio, halfKnee);
    } else {
        unlock();
        return gain;
    }
}

void dsp::CompressorGate::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    state.resize(numChannels, 0.0);
}

void dsp::CompressorGate::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *controlChannel = getControl()->getWrapper().getChannelPointer(channel);
        Sample *thresholdChannel = getThreshold()->getWrapper().getChannelPointer(channel);
        Sample *halfKneeChannel = getHalfKnee()->getWrapper().getChannelPointer(channel);
        Sample *compressionRatioChannel = getCompressionRatio()->getWrapper().getChannelPointer(channel);
        Sample *gateRatioChannel = getGateRatio()->getWrapper().getChannelPointer(channel);
        Sample *attackChannel = getAttack()->getWrapper().getChannelPointer(channel);
        Sample *releaseChannel = getRelease()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample *gainResponseChannel = getGainResponse()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample &input = inputChannel[sample];
            Sample &control = controlChannel[sample];
            Sample &threshold = thresholdChannel[sample];
            Sample &halfKnee = halfKneeChannel[sample];
            Sample &compressionRatio = compressionRatioChannel[sample];
            Sample &gateRatio = gateRatioChannel[sample];
            Sample &attack = attackChannel[sample];
            Sample &release = releaseChannel[sample];
            Sample &output = outputChannel[sample];
            Sample &gainResponse = gainResponseChannel[sample];
            if (isnan(state[channel])) {
                state[channel] = 0.0;
            }
            const Sample gain = log2(abs(control));
            const Sample gainResponseTarget = getGainResponse(gain, threshold, halfKnee, compressionRatio, gateRatio);
            const Sample decay = (abs(gainResponseTarget) > abs(state[channel]) ? attack : release) * getSampleRate();
            state[channel] = gainResponseTarget + pow(0.001, 1.0 / decay) * (state[channel] - gainResponseTarget);
            gainResponse = state[channel];
            output = input * exp2(gainResponse);
        }
    }
}

dsp::Sample dsp::CompressorGate::getGainResponse(const Sample &gain,
                                                 const Sample &threshold,
                                                 const Sample &halfKnee,
                                                 const Sample &compressionRatio,
                                                 const Sample &gateRatio) {
    const Sample gainMinusThreshold = gain - threshold;
    if (isinf(gainMinusThreshold)) {
        return 0.0;
    }
    const Sample posHalfKnee = abs(halfKnee);
    const Sample ratioDifference = 1.0 / compressionRatio - gateRatio;
    if (gainMinusThreshold > -posHalfKnee && gainMinusThreshold < posHalfKnee && ratioDifference != 0.0) {
        if (compressionRatio == 0.0 || isinf(gateRatio) || isinf(posHalfKnee)) {
            return -std::numeric_limits<double>::infinity();
        }
        const Sample knee = 2.0 * posHalfKnee;
        const Sample kneeTimesGR = knee * gateRatio;
        const Sample t = gainMinusThreshold / knee + 0.5;
        return t * (t * posHalfKnee * ratioDifference + kneeTimesGR) - 0.5 * kneeTimesGR - gainMinusThreshold;
    } else {
        return gainMinusThreshold * (gainMinusThreshold > 0.0 ? 1.0 / compressionRatio - 1.0 : gateRatio - 1.0);
    }
}
