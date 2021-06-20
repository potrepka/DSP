#include "Buffer.h"

dsp::Buffer::Buffer(Type type, Space space, Sample range, Sample defaultValue, size_t numChannels, size_t numSamples)
        : type(type)
        , space(space)
        , range(range)
        , defaultValue(defaultValue)
        , data(numChannels, numSamples)
        , wrapper(data)
        , channelValues(numChannels, defaultValue) {
    wrapper.fill(defaultValue);
}

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

dsp::Sample dsp::Buffer::getRange() const {
    return range;
}

void dsp::Buffer::setRange(Sample range) {
    lock();
    this->range = range;
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

size_t dsp::Buffer::getNumChannels() const {
    return data.getNumChannels();
}

void dsp::Buffer::setNumChannels(size_t numChannels) {
    lock();
    data.setSize(numChannels, getNumSamples());
    wrapper = Wrapper(data);
    channelValues.resize(numChannels, defaultValue);
    fillChannels();
    unlock();
}

size_t dsp::Buffer::getNumSamples() const {
    return data.getNumSamples();
}

void dsp::Buffer::setNumSamples(size_t numSamples) {
    lock();
    data.setSize(getNumChannels(), numSamples);
    wrapper = Wrapper(data);
    fillChannels();
    unlock();
}

void dsp::Buffer::setSize(size_t numChannels, size_t numSamples) {
    lock();
    data.setSize(numChannels, numSamples);
    wrapper = Wrapper(data);
    channelValues.resize(numChannels, defaultValue);
    fillChannels();
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

dsp::Sample dsp::Buffer::getChannelValue(size_t channel) const {
    return channelValues[channel];
}

void dsp::Buffer::setSingleChannelValue(size_t channel, Sample value) {
    lock();
    DSP_ASSERT(channel < getNumChannels());
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
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        peak[channel] = data.getMagnitude(channel, 0, getNumSamples());
    }
    unlock();
    return peak;
}

dsp::Array dsp::Buffer::getRMS() {
    lock();
    Array rms(getNumChannels());
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        rms[channel] = data.getRMSLevel(channel, 0, getNumSamples());
    }
    unlock();
    return rms;
}

dsp::Wrapper &dsp::Buffer::getWrapper() {
    return wrapper;
}

void dsp::Buffer::fillChannels() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        wrapper.getSingleChannel(channel).fill(channelValues[channel]);
    }
}

void dsp::Buffer::applyRange() {
    if (range > 0.0) {
        wrapper.replaceWithMaxOf(wrapper, 0.0);
        wrapper.replaceWithMinOf(wrapper, range);
    }
}

dsp::Input::Input(Type type, Space space, Sample range, Sample defaultValue, size_t numChannels, size_t numSamples)
        : Buffer(type, space, range, defaultValue, numChannels, numSamples)
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

void dsp::Input::prepareNoLock() {
    if (connections.size() == 0) {
        fillChannels();
    } else {
        switch (mode) {
            case Mode::SUM: wrapper.fill(0.0); break;
            case Mode::MINIMUM: wrapper.fill(std::numeric_limits<Sample>::infinity()); break;
            case Mode::MAXIMUM: wrapper.fill(-std::numeric_limits<Sample>::infinity()); break;
        }
    }
}

void dsp::Input::processNoLock() {
    for (const auto &output : connections) {
        switch (mode) {
            case Mode::SUM: wrapper.add(output->getWrapper()); break;
            case Mode::MINIMUM: wrapper.replaceWithMinOf(wrapper, output->getWrapper()); break;
            case Mode::MAXIMUM: wrapper.replaceWithMaxOf(wrapper, output->getWrapper()); break;
        }
    }
    switch (type) {
        case Type::INTEGER: wrapper.apply([](Sample x) { return floor(x); }); break;
        case Type::BOOLEAN: wrapper.apply([](Sample x) { return static_cast<int>(floor(x)) & 1; }); break;
    }
    applyRange();
}

void dsp::Input::addConnection(std::shared_ptr<Output> output) {
    if (std::find(connections.begin(), connections.end(), output) == connections.end()) {
        connections.push_back(output);
    }
}

void dsp::Input::removeConnection(std::shared_ptr<Output> output) {
    connections.erase(std::remove(connections.begin(), connections.end(), output), connections.end());
}

dsp::Output::Output(Type type, Space space, Sample range, Sample defaultValue, size_t numChannels, size_t numSamples)
        : Buffer(type, space, range, defaultValue, numChannels, numSamples) {}

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

void dsp::Output::prepareNoLock() {
    fillChannels();
}

void dsp::Output::processNoLock() {
    switch (type) {
        case Type::INTEGER: wrapper.apply([](Sample x) { return floor(x); }); break;
        case Type::BOOLEAN: wrapper.apply([](Sample x) { return x ? 1.0 : 0.0; }); break;
    }
    applyRange();
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
    input->setDefaultValue(value);
    input->setAllChannelValues(value);
}

void dsp::operator>>(Sample value, std::shared_ptr<Output> output) {
    output->setDefaultValue(value);
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
