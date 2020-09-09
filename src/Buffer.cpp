#include "Buffer.h"

dsp::Buffer::Buffer(Type type, Space space, Sample defaultValue, int numChannels, int numSamples)
        : type(type)
        , space(space)
        , defaultValue(defaultValue)
        , data(numChannels, numSamples)
        , block(data)
        , channelValues(numChannels, defaultValue) {}

dsp::Type dsp::Buffer::getType() const {
    return type;
}

void dsp::Buffer::setType(Type type) {
    lock();
    this->type = type;
    unlock();
}

dsp::Space dsp::Buffer::getSpace() const {
    return space;
}

void dsp::Buffer::setSpace(Space space) {
    lock();
    this->space = space;
    unlock();
}

dsp::Sample dsp::Buffer::getDefaultValue() const {
    return defaultValue;
}

void dsp::Buffer::setDefaultValue(Sample defaultValue) {
    lock();
    this->defaultValue = defaultValue;
    unlock();
}

int dsp::Buffer::getNumChannels() const {
    return data.getNumChannels();
}

void dsp::Buffer::setNumChannels(int numChannels) {
    DSP_ASSERT(numChannels >= 0);
    lock();
    data.setSize(numChannels, getNumSamples());
    block = Block(data);
    channelValues.resize(numChannels, defaultValue);
    unlock();
}

int dsp::Buffer::getNumSamples() const {
    return data.getNumSamples();
}

void dsp::Buffer::setNumSamples(int numSamples) {
    DSP_ASSERT(numSamples >= 0);
    lock();
    data.setSize(getNumChannels(), numSamples);
    block = Block(data);
    unlock();
}

void dsp::Buffer::setSize(int numChannels, int numSamples) {
    DSP_ASSERT(numChannels >= 0 && numSamples >= 0);
    lock();
    data.setSize(numChannels, numSamples);
    block = Block(data);
    channelValues.resize(numChannels, defaultValue);
    unlock();
}

dsp::Array dsp::Buffer::getChannelValues() const {
    return channelValues;
}

void dsp::Buffer::setChannelValues(Array values) {
    lock();
    DSP_ASSERT(values.size() == getNumChannels());
    channelValues = values;
    unlock();
}

dsp::Sample dsp::Buffer::getChannelValue(int channel) const {
    return channelValues[channel];
}

void dsp::Buffer::setSingleChannelValue(int channel, Sample value) {
    lock();
    DSP_ASSERT(channel >= 0 && channel < getNumChannels());
    channelValues[channel] = value;
    unlock();
}

void dsp::Buffer::setAllChannelValues(Sample value) {
    lock();
    std::fill(channelValues.begin(), channelValues.end(), value);
    unlock();
}

dsp::Array dsp::Buffer::getPeak() {
    lock();
    Array peak(getNumChannels());
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        peak[channel] = data.getMagnitude(channel, 0, getNumSamples());
    }
    unlock();
    return peak;
}

dsp::Array dsp::Buffer::getRMS() {
    lock();
    Array rms(getNumChannels());
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        rms[channel] = data.getRMSLevel(channel, 0, getNumSamples());
    }
    unlock();
    return rms;
}

dsp::Block &dsp::Buffer::getBlock() {
    return block;
}

void dsp::Buffer::fillChannels() {
    for (int channel = 0; channel < getNumChannels(); ++channel) {
        block.getSingleChannelBlock(channel).fill(channelValues[channel]);
    }
}

dsp::Input::Input(Type type, Space space, Sample defaultValue, int numChannels, int numSamples)
        : Buffer(type, space, defaultValue, numChannels, numSamples)
        , mode(Mode::SUM) {}

dsp::Input::~Input() {
    disconnectAll();
}

dsp::Input::Mode dsp::Input::getMode() const {
    return mode;
}

void dsp::Input::setMode(Mode mode) {
    lock();
    this->mode = mode;
    unlock();
}

std::vector<std::shared_ptr<dsp::Output>> dsp::Input::getConnections() const {
    return connections;
}

void dsp::Input::connect(std::shared_ptr<Output> output) {
    DSP_ASSERT(output != nullptr);

    lock();
    output->lock();

    addConnection(output);
    output->addConnection(shared_from_this());

    output->unlock();
    unlock();
}

void dsp::Input::disconnect(std::shared_ptr<Output> output) {
    DSP_ASSERT(output != nullptr);

    lock();
    output->lock();

    removeConnection(output);
    output->removeConnection(shared_from_this());

    output->unlock();
    unlock();
}

void dsp::Input::disconnectAll() {
    lock();
    for (const auto &output : connections) {
        output->lock();
    }
    for (const auto &output : connections) {
        output->removeConnection(shared_from_this());
    }
    for (const auto &output : connections) {
        output->unlock();
    }
    connections.clear();
    unlock();
}

void dsp::Input::processNoLock() {
    if (connections.size() == 0) {
        fillChannels();
    } else {
        switch (mode) {
            case Mode::SUM: block.fill(0.0); break;
            case Mode::MINIMUM: block.fill(std::numeric_limits<Sample>::infinity()); break;
            case Mode::MAXIMUM: block.fill(-std::numeric_limits<Sample>::infinity()); break;
        }
    }
    for (const auto &output : connections) {
        switch (mode) {
            case Mode::SUM: block.add(output->getBlock()); break;
            case Mode::MINIMUM: block.replaceWithMinOf(block, output->getBlock()); break;
            case Mode::MAXIMUM: block.replaceWithMaxOf(block, output->getBlock()); break;
        }
    }
}

void dsp::Input::addConnection(std::shared_ptr<Output> output) {
    if (std::find(connections.begin(), connections.end(), output) == connections.end()) {
        connections.push_back(output);
    }
}

void dsp::Input::removeConnection(std::shared_ptr<Output> output) {
    connections.erase(std::remove(connections.begin(), connections.end(), output), connections.end());
}

dsp::Output::Output(Type type, Space space, Sample defaultValue, int numChannels, int numSamples)
        : Buffer(type, space, defaultValue, numChannels, numSamples) {}

dsp::Output::~Output() {
    disconnectAll();
}

std::vector<std::shared_ptr<dsp::Input>> dsp::Output::getConnections() const {
    return connections;
}

void dsp::Output::connect(std::shared_ptr<Input> input) {
    DSP_ASSERT(input != nullptr);

    lock();
    input->lock();

    addConnection(input);
    input->addConnection(shared_from_this());

    input->unlock();
    unlock();
}

void dsp::Output::disconnect(std::shared_ptr<Input> input) {
    DSP_ASSERT(input != nullptr);

    lock();
    input->lock();

    removeConnection(input);
    input->removeConnection(shared_from_this());

    input->unlock();
    unlock();
}

void dsp::Output::disconnectAll() {
    lock();
    for (const auto &input : connections) {
        input->lock();
    }
    for (const auto &input : connections) {
        input->removeConnection(shared_from_this());
    }
    for (const auto &input : connections) {
        input->unlock();
    }
    connections.clear();
    unlock();
}

void dsp::Output::processNoLock() {
    fillChannels();
}

void dsp::Output::addConnection(std::shared_ptr<Input> input) {
    if (std::find(connections.begin(), connections.end(), input) == connections.end()) {
        connections.push_back(input);
    }
}

void dsp::Output::removeConnection(std::shared_ptr<Input> input) {
    connections.erase(std::remove(connections.begin(), connections.end(), input), connections.end());
}

void dsp::operator>>(Sample value, std::shared_ptr<Input> input) {
    input->setAllChannelValues(value);
}

void dsp::operator>>(Sample value, std::shared_ptr<Output> output) {
    output->setAllChannelValues(value);
}

void dsp::operator>>(Array values, std::shared_ptr<Input> input) {
    input->setChannelValues(values);
}

void dsp::operator>>(Array values, std::shared_ptr<Output> output) {
    output->setChannelValues(values);
}

void dsp::operator>>(std::shared_ptr<Output> output, std::shared_ptr<Input> input) {
    output->connect(input);
}

void dsp::operator!=(std::shared_ptr<Output> output, std::shared_ptr<Input> input) {
    input->disconnect(output);
}
