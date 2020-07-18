#pragma once

#include "Buffer.h"
#include "Generator.h"

namespace dsp {

class SamplePlayer : public Generator, public BufferCollection {

public:
    enum class Mode { ONE_SHOT, LOOP };

    SamplePlayer(Type type);

    Mode getMode() const;
    void setMode(Mode mode);
    unsigned int getNumSamples() const;
    std::shared_ptr<Buffer> getSample(unsigned int index) const;
    std::vector<std::shared_ptr<Buffer>> getSamples(unsigned int begin, unsigned int end) const;
    void pushSample(std::shared_ptr<Buffer> sample);
    void pushSamples(std::vector<std::shared_ptr<Buffer>> samples);
    void replaceSample(std::shared_ptr<Buffer> sample, std::shared_ptr<Buffer> replacement);
    void removeSample(std::shared_ptr<Buffer> sample);
    void removeSamples(std::vector<std::shared_ptr<Buffer>> samples);
    std::shared_ptr<InputParameter> getResetTrigger() const;
    std::shared_ptr<InputParameter> getGate() const;
    std::shared_ptr<InputParameter> getOffsetTime() const;
    std::shared_ptr<InputParameter> getSpeed() const;
    std::shared_ptr<InputParameter> getSampleIndex() const;
    std::shared_ptr<OutputParameter> getCurrentTime() const;

protected:
    void setNumChannelsNoLock(unsigned int numChannels) override;
    void process() override;

private:
    const std::shared_ptr<InputParameter> resetTrigger;
    const std::shared_ptr<InputParameter> gate;
    const std::shared_ptr<InputParameter> offsetTime;
    const std::shared_ptr<InputParameter> speed;
    const std::shared_ptr<InputParameter> sampleIndex;

    const std::shared_ptr<OutputParameter> currentTime;

    Mode mode;
    std::vector<DSP_FLOAT> readIndex;
};

} // namespace dsp
