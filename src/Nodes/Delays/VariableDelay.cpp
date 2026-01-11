#include "VariableDelay.h"

dsp::VariableDelay::VariableDelay(Type type)
    : Transformer(type, type),
      maxDelayTime(0.0),
      buffer(std::make_shared<Buffer>(type)),
      delayTime(std::make_shared<Input>(Type::SECONDS)),
      decayTime(std::make_shared<Input>(Type::SECONDS)),
      reset(std::make_shared<Input>(Type::BOOLEAN)),
      feedbackSource(std::make_shared<Output>(type)),
      feedbackSink(std::make_shared<Input>(type)),
      feedbackProcessor(std::make_shared<Node>()),
      writeIndex(0) {
  getInputs().push_back(delayTime);
  getInputs().push_back(decayTime);
  getInputs().push_back(reset);
}

dsp::Sample dsp::VariableDelay::getMaxDelayTime() const { return maxDelayTime; }

void dsp::VariableDelay::setMaxDelayTime(Sample seconds) {
  DSP_ASSERT(seconds >= 0.0);
  lock();
  maxDelayTime = seconds;
  writeIndex = 0;
  buffer->setNumSamples(getDelayBufferSize());
  buffer->getWrapper().clear();
  unlock();
}

std::shared_ptr<dsp::Input> dsp::VariableDelay::getDelayTime() const {
  return delayTime;
}

std::shared_ptr<dsp::Input> dsp::VariableDelay::getDecayTime() const {
  return decayTime;
}

std::shared_ptr<dsp::Input> dsp::VariableDelay::getReset() const {
  return reset;
}

std::shared_ptr<dsp::Output> dsp::VariableDelay::getFeedbackSource() const {
  return feedbackSource;
}

std::shared_ptr<dsp::Input> dsp::VariableDelay::getFeedbackSink() const {
  return feedbackSink;
}

std::shared_ptr<dsp::Node> dsp::VariableDelay::getFeedbackProcessor() const {
  return feedbackProcessor;
}

void dsp::VariableDelay::setNumInputChannelsNoLock(size_t numChannels) {
  Node::setNumInputChannelsNoLock(numChannels);
}

void dsp::VariableDelay::setNumOutputChannelsNoLock(size_t numChannels) {
  Node::setNumOutputChannelsNoLock(numChannels);
  buffer->setNumChannels(numChannels);
  feedbackSource->setNumChannels(numChannels);
  feedbackSink->setNumChannels(numChannels);
}

void dsp::VariableDelay::setNumSamplesNoLock(size_t numSamples) {
  Node::setNumSamplesNoLock(numSamples);
  feedbackSource->setNumSamples(1);
  feedbackSink->setNumSamples(1);
  feedbackProcessor->setNumSamples(1);
}

void dsp::VariableDelay::setSampleRateNoLock(double sampleRate) {
  Node::setSampleRateNoLock(sampleRate);
  writeIndex = 0;
  buffer->setNumSamples(getDelayBufferSize());
  buffer->getWrapper().clear();
}

void dsp::VariableDelay::processNoLock() {
  feedbackSource->lock();
  feedbackSink->lock();
  feedbackProcessor->lock();
  if (getBuffer()->getNumSamples() > 0) {
    Channels ch = getChannels();
    for (size_t sample = 0; sample < getNumSamples(); ++sample) {
      writeInputToBuffer(ch, sample);
      readOutputFromBuffer(ch, sample);
      processFeedback();
      addFeedbackToBuffer(ch, sample);
      incrementWriteIndex();
    }
  }
  feedbackProcessor->unlock();
  feedbackSink->unlock();
  feedbackSource->unlock();
}

const std::shared_ptr<dsp::Buffer> dsp::VariableDelay::getBuffer() const {
  return buffer;
}

size_t dsp::VariableDelay::getDelayBufferSize() {
  return static_cast<size_t>(ceil(maxDelayTime * getSampleRate())) + 2;
}

dsp::VariableDelay::Channels dsp::VariableDelay::getChannels() {
  Channels ch;
  size_t numChannels = getNumChannels();
  ch.clippedDelayTime.resize(numChannels);
  ch.buffer.resize(numChannels);
  ch.input.resize(numChannels);
  ch.delayTime.resize(numChannels);
  ch.decayTime.resize(numChannels);
  ch.reset.resize(numChannels);
  ch.output.resize(numChannels);
  ch.feedbackSource.resize(numChannels);
  ch.feedbackSink.resize(numChannels);
  for (size_t channel = 0; channel < numChannels; ++channel) {
    ch.buffer[channel] = getBuffer()->getWrapper().getChannelPointer(channel);
    ch.input[channel] = getInput()->getWrapper().getChannelPointer(channel);
    ch.delayTime[channel] =
        getDelayTime()->getWrapper().getChannelPointer(channel);
    ch.decayTime[channel] =
        getDecayTime()->getWrapper().getChannelPointer(channel);
    ch.reset[channel] = getReset()->getWrapper().getChannelPointer(channel);
    ch.output[channel] = getOutput()->getWrapper().getChannelPointer(channel);
    ch.feedbackSource[channel] =
        getFeedbackSource()->getWrapper().getChannelPointer(channel);
    ch.feedbackSink[channel] =
        getFeedbackSink()->getWrapper().getChannelPointer(channel);
  }
  return ch;
}

void dsp::VariableDelay::writeInputToBuffer(Channels& ch, size_t sample) {
  for (size_t channel = 0; channel < getNumChannels(); ++channel) {
    if (ch.reset[channel][sample]) {
      getBuffer()->getWrapper().getSingleChannel(channel).clear();
    }
    ch.buffer[channel][writeIndex] = ch.input[channel][sample];
    ch.clippedDelayTime[channel] =
        clip(ch.delayTime[channel][sample], 0.0, maxDelayTime);
  }
}

void dsp::VariableDelay::readOutputFromBuffer(Channels& ch, size_t sample) {
  for (size_t channel = 0; channel < getNumChannels(); ++channel) {
    Sample delayTime = ch.clippedDelayTime[channel];
    if (delayTime == 0.0) {
      ch.output[channel][sample] = ch.buffer[channel][writeIndex];
    } else {
      Sample readIndex = writeIndex - delayTime * getSampleRate();
      if (readIndex < 0.0) {
        readIndex += getBuffer()->getNumSamples();
      }
      size_t bufferSize = getBuffer()->getNumSamples();
      if (delayTime <= getOneOverSampleRate()) {
        ch.output[channel][sample] =
            linearWrapped(ch.buffer[channel], bufferSize, readIndex);
      } else {
        ch.output[channel][sample] =
            hermiteWrapped(ch.buffer[channel], bufferSize, readIndex);
      }
    }
    ch.feedbackSource[channel][0] = ch.output[channel][sample];
  }
}

void dsp::VariableDelay::processFeedback() {
  if (feedbackProcessor->isActive()) {
    for (const auto& child : feedbackProcessor->getChildren()) {
      child->process();
    }
  }
  getFeedbackSink()->processNoLock();
}

void dsp::VariableDelay::addFeedbackToBuffer(Channels& ch, size_t sample) {
  for (size_t channel = 0; channel < getNumChannels(); ++channel) {
    Sample feedbackAmount = pow(
        0.001, ch.clippedDelayTime[channel] / ch.decayTime[channel][sample]);
    if (isnan(feedbackAmount)) {
      feedbackAmount = 0.0;
    }
    if (feedbackAmount > 1.0) {
      feedbackAmount = 1.0;
    }
    ch.buffer[channel][writeIndex] +=
        ch.feedbackSink[channel][0] * feedbackAmount;
  }
}

void dsp::VariableDelay::incrementWriteIndex() {
  ++writeIndex;
  if (writeIndex == getBuffer()->getNumSamples()) {
    writeIndex = 0;
  }
}
