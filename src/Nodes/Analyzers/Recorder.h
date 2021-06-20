#pragma once

#include "../Core/Consumer.h"

namespace dsp {

class Recorder : public Consumer {

public:
    enum class Mode { FIXED_SINGLE, FIXED_DOUBLE, VARIABLE };

    Recorder(Type type = Type::RATIO, Space space = Space::TIME, Sample defaultValue = 0.0);

    Mode getMode() const;
    void setMode(Mode mode);

    Sample getRecordingTime() const;
    void setRecordingTime(Sample seconds);
    std::shared_ptr<Buffer> getRecordingBuffer() const;

    std::shared_ptr<Input> getGate() const;
    std::shared_ptr<Input> getReset() const;

protected:
    void setNumInputChannelsNoLock(size_t numChannels) override;
    void setSampleRateNoLock(double sampleRate) override;
    void processNoLock() override;

private:
    Mode mode;
    Sample recordingTime;
    Sample recordingNumSamples;
    Array writeIndex;
    const std::shared_ptr<Buffer> primary;
    const std::shared_ptr<Buffer> secondary;
    const std::shared_ptr<Input> gate;
    const std::shared_ptr<Input> reset;

    void setRecordingTimeNoLock(Sample seconds);
};

} // namespace dsp
