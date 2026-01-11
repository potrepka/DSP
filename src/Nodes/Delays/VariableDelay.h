#pragma once

#include "../../Core/Transformer.h"

namespace dsp {

class VariableDelay : public Transformer {
public:
  VariableDelay(Type type = Type::RATIO);

  Sample getMaxDelayTime() const;
  void setMaxDelayTime(Sample seconds);

  std::shared_ptr<Input> getDelayTime() const;
  std::shared_ptr<Input> getDecayTime() const;
  std::shared_ptr<Input> getReset() const;
  std::shared_ptr<Output> getFeedbackSource() const;
  std::shared_ptr<Input> getFeedbackSink() const;

  std::shared_ptr<Node> getFeedbackProcessor() const;

protected:
  void setNumInputChannelsNoLock(size_t numChannels) override;
  void setNumOutputChannelsNoLock(size_t numChannels) override;
  void setNumSamplesNoLock(size_t numSamples) override;
  void setSampleRateNoLock(double sampleRate) override;
  void processNoLock() override;

private:
  struct Channels {
    std::vector<Sample> clippedDelayTime;
    std::vector<Sample*> buffer;
    std::vector<Sample*> input;
    std::vector<Sample*> delayTime;
    std::vector<Sample*> decayTime;
    std::vector<Sample*> reset;
    std::vector<Sample*> output;
    std::vector<Sample*> feedbackSource;
    std::vector<Sample*> feedbackSink;
  };

  Sample maxDelayTime;
  const std::shared_ptr<Buffer> buffer;
  const std::shared_ptr<Input> delayTime;
  const std::shared_ptr<Input> decayTime;
  const std::shared_ptr<Input> reset;
  const std::shared_ptr<Output> feedbackSource;
  const std::shared_ptr<Input> feedbackSink;
  const std::shared_ptr<Node> feedbackProcessor;
  size_t writeIndex;

  const std::shared_ptr<Buffer> getBuffer() const;
  size_t getDelayBufferSize();
  Channels getChannels();
  void writeInputToBuffer(Channels& ch, size_t sample);
  void readOutputFromBuffer(Channels& ch, size_t sample);
  void processFeedback();
  void addFeedbackToBuffer(Channels& ch, size_t sample);
  void incrementWriteIndex();
};

} // namespace dsp
