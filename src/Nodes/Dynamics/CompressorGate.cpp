#include "CompressorGate.h"

dsp::CompressorGate::CompressorGate()
        : Transformer(Type::RATIO, Type::RATIO)
        , control(std::make_shared<Input>(Type::RATIO))
        , threshold(std::make_shared<Input>(Type::RATIO, Space::TIME, 0.0, 1.0))
        , hardness(std::make_shared<Input>(Type::RATIO, Space::TIME, 1.0, 1.0))
        , compressionRatio(std::make_shared<Input>(Type::RATIO, Space::TIME, 0.0, 1.0))
        , gateRatio(std::make_shared<Input>(Type::RATIO, Space::TIME, 0.0, 1.0))
        , attack(std::make_shared<Input>(Type::SECONDS))
        , release(std::make_shared<Input>(Type::SECONDS))
        , gain(std::make_shared<Output>(Type::RATIO, Space::TIME, 0.0, 1.0)) {
    getInputs().push_back(control);
    getInputs().push_back(threshold);
    getInputs().push_back(hardness);
    getInputs().push_back(compressionRatio);
    getInputs().push_back(gateRatio);
    getInputs().push_back(attack);
    getInputs().push_back(release);
    getOutputs().push_back(gain);
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getControl() const {
    return control;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getThreshold() const {
    return threshold;
}

std::shared_ptr<dsp::Input> dsp::CompressorGate::getHardness() const {
    return hardness;
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

std::shared_ptr<dsp::Output> dsp::CompressorGate::getGain() const {
    return gain;
}

dsp::Sample dsp::CompressorGate::getGainResponse(size_t channel, Sample control) {
    lock();
    DSP_ASSERT(channel < getNumChannels());
    if (getNumSamples() > 0) {
        const size_t lastSample = getNumSamples() - 1;
        const Sample threshold = getThreshold()->getWrapper().getSample(channel, lastSample);
        const Sample hardness = getHardness()->getWrapper().getSample(channel, lastSample);
        const Sample compressionRatio = getCompressionRatio()->getWrapper().getSample(channel, lastSample);
        const Sample gateRatio = getGateRatio()->getWrapper().getSample(channel, lastSample);
        unlock();
        return control * getGainResponse(control, threshold, hardness, compressionRatio, gateRatio);
    } else {
        unlock();
        return control;
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
        Sample *hardnessChannel = getHardness()->getWrapper().getChannelPointer(channel);
        Sample *compressionRatioChannel = getCompressionRatio()->getWrapper().getChannelPointer(channel);
        Sample *gateRatioChannel = getGateRatio()->getWrapper().getChannelPointer(channel);
        Sample *attackChannel = getAttack()->getWrapper().getChannelPointer(channel);
        Sample *releaseChannel = getRelease()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample *gainChannel = getGain()->getWrapper().getChannelPointer(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            Sample &input = inputChannel[sample];
            Sample &control = controlChannel[sample];
            Sample &threshold = thresholdChannel[sample];
            Sample &hardness = hardnessChannel[sample];
            Sample &compressionRatio = compressionRatioChannel[sample];
            Sample &gateRatio = gateRatioChannel[sample];
            Sample &attack = attackChannel[sample];
            Sample &release = releaseChannel[sample];
            Sample &output = outputChannel[sample];
            Sample &gain = gainChannel[sample];
            if (isnan(state[channel])) {
                state[channel] = 0.0;
            }
            const Sample gainTarget = getGainResponse(control, threshold, hardness, compressionRatio, gateRatio);
            Sample decay;
            if (gainTarget > 1.0 && gainTarget > state[channel] ||
                gainTarget < 1.0 && gainTarget < state[channel]) {
                decay = attack * getSampleRate();
            } else {
                decay = release * getSampleRate();
            }
            state[channel] = gainTarget + pow(0.001, 1.0 / decay) * (state[channel] - gainTarget);
            gain = state[channel];
            output = input * gain;
        }
    }
}

dsp::Sample dsp::CompressorGate::getGainResponse(const Sample &control,
                                                 const Sample &threshold,
                                                 const Sample &hardness,
                                                 const Sample &compressionRatio,
                                                 const Sample &gateRatio) {

    const Sample controlMinusThreshold = log2(abs(control)) - log2(abs(threshold));
    if (isinf(controlMinusThreshold)) {
        return 1.0;
    }
    Sample logGain;
    const Sample halfKnee = log2(abs(hardness));
    const Sample ratioDifference = 1.0 / compressionRatio - gateRatio;
    if (controlMinusThreshold > -halfKnee && controlMinusThreshold < halfKnee && ratioDifference != 0.0) {
        if (compressionRatio == 0.0 || isinf(gateRatio) || isinf(halfKnee)) {
            return 0.0;
        }
        const Sample knee = 2.0 * halfKnee;
        const Sample kneeTimesGR = knee * gateRatio;
        const Sample t = controlMinusThreshold / knee + 0.5;
        logGain = t * (t * halfKnee * ratioDifference + kneeTimesGR) - 0.5 * kneeTimesGR - controlMinusThreshold;
    } else {
        const Sample ratioMinusOne = controlMinusThreshold > 0.0 ? 1.0 / compressionRatio - 1.0 : gateRatio - 1.0;
        logGain = controlMinusThreshold * ratioMinusOne;
    }
    return exp2(logGain);
}
