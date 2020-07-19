#include "Connection.h"

dsp::Connection::Connection(unsigned int bufferSize, Type type, Space space, Sample defaultValue)
        : type(type)
        , space(space)
        , defaultValue(defaultValue) {
    setBufferSize(bufferSize);
}

unsigned int dsp::Connection::getBufferSize() const {
    return static_cast<unsigned int>(buffer.size());
}

void dsp::Connection::setBufferSize(unsigned int bufferSize) {
    lock();
    buffer.resize(bufferSize);
    unlock();
}

dsp::Type dsp::Connection::getType() const {
    return type;
}

void dsp::Connection::setType(Type type) {
    lock();
    this->type = type;
    unlock();
}

dsp::Space dsp::Connection::getSpace() const {
    return space;
}

void dsp::Connection::setSpace(Space space) {
    lock();
    this->space = space;
    unlock();
}

dsp::Sample dsp::Connection::getDefaultValue() const {
    return defaultValue;
}

void dsp::Connection::setDefaultValue(Sample defaultValue) {
    lock();
    this->defaultValue = defaultValue;
    unlock();
}

void dsp::Connection::fillBuffer(Sample value) {
    lock();
    std::fill(buffer.begin(), buffer.end(), value);
    unlock();
}

void dsp::Connection::clearBuffer() {
    lock();
    std::fill(buffer.begin(), buffer.end(), defaultValue);
    unlock();
}

dsp::Array &dsp::Connection::getBuffer() {
    return buffer;
}

dsp::Input::Input(unsigned int bufferSize, Type type, Space space, Sample defaultValue)
        : Connection(bufferSize, type, space, defaultValue) {}

dsp::Input::~Input() {
    disconnectAll();
}

std::vector<std::shared_ptr<dsp::Output>> dsp::Input::getConnections() const {
    return connections;
}

void dsp::Input::connect(std::shared_ptr<Output> output) {
    assert(output != nullptr);

    lock();
    output->lock();

    output->addConnection(shared_from_this());
    addConnection(output);

    output->unlock();
    unlock();
}

void dsp::Input::disconnect(std::shared_ptr<Output> output) {
    assert(output != nullptr);

    lock();
    output->lock();

    output->removeConnection(shared_from_this());
    removeConnection(output);

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

void dsp::Input::copyBuffers() {
    lock();
    if (connections.size() == 0) {
        for (Iterator it = buffer.begin(); it != buffer.end(); ++it) {
            *it = Vector(defaultValue);
        }
    } else {
        for (std::size_t i = 0; i < connections.size(); ++i) {
            iterators[i] = connections[i]->getBuffer().begin();
        }
        for (Iterator it = buffer.begin(); it != buffer.end(); ++it) {
            Vector sum = Vector(0.0);
            for (Iterator &iterator : iterators) {
                sum += *iterator;
                ++iterator;
            }
            *it = sum;
        }
    }
    unlock();
}

void dsp::Input::addConnection(std::shared_ptr<Output> output) {
    if (std::find(connections.begin(), connections.end(), output) == connections.end()) {
        connections.push_back(output);
        iterators.resize(connections.size());
    }
}

void dsp::Input::removeConnection(std::shared_ptr<Output> output) {
    connections.erase(std::remove(connections.begin(), connections.end(), output), connections.end());
    iterators.resize(connections.size());
}

dsp::Output::Output(unsigned int bufferSize, Type type, Space space, Sample defaultValue)
        : Connection(bufferSize, type, space, defaultValue) {}

dsp::Output::~Output() {
    disconnectAll();
}

std::vector<std::shared_ptr<dsp::Input>> dsp::Output::getConnections() const {
    return connections;
}

void dsp::Output::connect(std::shared_ptr<Input> input) {
    assert(input != nullptr);

    lock();
    input->lock();

    input->addConnection(shared_from_this());
    addConnection(input);

    input->unlock();
    unlock();
}

void dsp::Output::disconnect(std::shared_ptr<Input> input) {
    assert(input != nullptr);

    lock();
    input->lock();

    input->removeConnection(shared_from_this());
    removeConnection(input);

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

void dsp::Output::addConnection(std::shared_ptr<Input> input) {
    if (std::find(connections.begin(), connections.end(), input) == connections.end()) {
        connections.push_back(input);
    }
}

void dsp::Output::removeConnection(std::shared_ptr<Input> input) {
    connections.erase(std::remove(connections.begin(), connections.end(), input), connections.end());
}

template <class T>
dsp::ConnectionParameter<T>::ConnectionParameter(
        unsigned int numChannels, unsigned int bufferSize, Type type, Space space, Sample defaultValue)
        : bufferSize(bufferSize)
        , type(type)
        , space(space)
        , defaultValue(defaultValue) {
    setNumChannels(numChannels);
}

template <class T> unsigned int dsp::ConnectionParameter<T>::getNumChannels() const {
    return static_cast<unsigned int>(channels.size());
}

template <class T> void dsp::ConnectionParameter<T>::setNumChannels(unsigned int numChannels) {
    lock();
    if (numChannels < channels.size()) {
        channels.erase(channels.begin() + numChannels, channels.end());
    } else {
        channels.reserve(numChannels);
        for (unsigned int i = getNumChannels(); i < numChannels; i++) {
            channels.push_back(std::make_shared<T>(bufferSize, type, space, defaultValue));
        }
    }
    unlock();
}

template <class T> unsigned int dsp::ConnectionParameter<T>::getBufferSize() const {
    return bufferSize;
}

template <class T> void dsp::ConnectionParameter<T>::setBufferSize(unsigned int bufferSize) {
    lock();
    this->bufferSize = bufferSize;
    for (const auto &channel : channels) {
        channel->setBufferSize(bufferSize);
    }
    unlock();
}

template <class T> dsp::Type dsp::ConnectionParameter<T>::getType() const {
    return type;
}

template <class T> void dsp::ConnectionParameter<T>::setType(Type type) {
    lock();
    this->type = type;
    for (const auto &channel : channels) {
        channel->setType(type);
    }
    unlock();
}

template <class T> dsp::Space dsp::ConnectionParameter<T>::getSpace() const {
    return space;
}

template <class T> void dsp::ConnectionParameter<T>::setSpace(Space space) {
    lock();
    this->space = space;
    for (const auto &channel : channels) {
        channel->setSpace(space);
    }
    unlock();
}

template <class T> dsp::Sample dsp::ConnectionParameter<T>::getDefaultValue() const {
    return defaultValue;
}

template <class T> void dsp::ConnectionParameter<T>::setDefaultValue(Sample defaultValue) {
    lock();
    this->defaultValue = defaultValue;
    for (const auto &channel : channels) {
        channel->setDefaultValue(defaultValue);
    }
    unlock();
}

template <class T> void dsp::ConnectionParameter<T>::fillBuffer(Sample value) {
    lock();
    for (const auto &channel : channels) {
        channel->fillBuffer(value);
    }
    unlock();
}

template <class T> void dsp::ConnectionParameter<T>::clearBuffer() {
    lock();
    for (const auto &channel : channels) {
        channel->clearBuffer();
    }
    unlock();
}

template <class T> std::vector<std::shared_ptr<T>> dsp::ConnectionParameter<T>::getChannels() const {
    return channels;
}

template <class T> std::shared_ptr<T> dsp::ConnectionParameter<T>::getChannel(unsigned int channel) const {
    assert(channel < channels.size());
    return channels[channel];
}

template class dsp::ConnectionParameter<dsp::Input>;

dsp::InputParameter::InputParameter(
        unsigned int numChannels, unsigned int bufferSize, Type type, Space space, Sample defaultValue)
        : ConnectionParameter(numChannels, bufferSize, type, space, defaultValue) {}

std::vector<std::shared_ptr<dsp::Output>> dsp::InputParameter::getConnections() {
    std::vector<std::shared_ptr<Output>> connections;
    lock();
    for (const auto &channel : channels) {
        connections.insert(connections.end(), channel->getConnections().begin(), channel->getConnections().end());
    }
    unlock();
    return connections;
}

void dsp::InputParameter::connect(std::shared_ptr<OutputParameter> output) {
    lock();
    output->lock();
    if (getNumChannels() > 0 && output->getNumChannels() > 0) {
        for (unsigned int i = 0; i < getNumChannels() || i < output->getNumChannels(); i++) {
            std::shared_ptr<Input> channel = getChannel(i % getNumChannels());
            std::shared_ptr<Output> outputChannel = output->getChannel(i % output->getNumChannels());
            channel->connect(outputChannel);
        }
    }
    output->unlock();
    unlock();
}

void dsp::InputParameter::disconnect(std::shared_ptr<OutputParameter> output) {
    lock();
    output->lock();
    if (getNumChannels() > 0 && output->getNumChannels() > 0) {
        for (unsigned int i = 0; i < getNumChannels() || i < output->getNumChannels(); i++) {
            std::shared_ptr<Input> channel = getChannel(i % getNumChannels());
            std::shared_ptr<Output> outputChannel = output->getChannel(i % output->getNumChannels());
            channel->disconnect(outputChannel);
        }
    }
    output->unlock();
    unlock();
}

void dsp::InputParameter::disconnectAll() {
    lock();
    for (const auto &channel : channels) {
        channel->disconnectAll();
    }
    unlock();
}

void dsp::InputParameter::copyBuffers() {
    lock();
    for (const auto &channel : channels) {
        channel->copyBuffers();
    }
    unlock();
}

template class dsp::ConnectionParameter<dsp::Output>;

dsp::OutputParameter::OutputParameter(
        unsigned int numChannels, unsigned int bufferSize, Type type, Space space, Sample defaultValue)
        : ConnectionParameter(numChannels, bufferSize, type, space, defaultValue) {}

std::vector<std::shared_ptr<dsp::Input>> dsp::OutputParameter::getConnections() {
    std::vector<std::shared_ptr<Input>> connections;
    lock();
    for (const auto &channel : channels) {
        connections.insert(connections.end(), channel->getConnections().begin(), channel->getConnections().end());
    }
    unlock();
    return connections;
}

void dsp::OutputParameter::connect(std::shared_ptr<InputParameter> input) {
    lock();
    input->lock();
    if (getNumChannels() > 0 && input->getNumChannels() > 0) {
        for (unsigned int i = 0; i < getNumChannels() || i < input->getNumChannels(); i++) {
            std::shared_ptr<Output> channel = getChannel(i % getNumChannels());
            std::shared_ptr<Input> inputChannel = input->getChannel(i % input->getNumChannels());
            channel->connect(inputChannel);
        }
    }
    input->unlock();
    unlock();
}

void dsp::OutputParameter::disconnect(std::shared_ptr<InputParameter> input) {
    lock();
    input->lock();
    if (getNumChannels() > 0 && input->getNumChannels() > 0) {
        for (unsigned int i = 0; i < getNumChannels() || i < input->getNumChannels(); i++) {
            std::shared_ptr<Output> channel = getChannel(i % getNumChannels());
            std::shared_ptr<Input> inputChannel = input->getChannel(i % input->getNumChannels());
            channel->disconnect(inputChannel);
        }
    }
    input->unlock();
    unlock();
}

void dsp::OutputParameter::disconnectAll() {
    lock();
    for (const auto &channel : channels) {
        channel->disconnectAll();
    }
    unlock();
}

void dsp::operator>>(Sample value, std::shared_ptr<Input> input) {
    input->setDefaultValue(value);
}

void dsp::operator>>(Sample value, std::shared_ptr<Output> output) {
    output->setDefaultValue(value);
}

void dsp::operator>>(std::shared_ptr<Output> output, std::shared_ptr<Input> input) {
    input->connect(output);
}

void dsp::operator!=(std::shared_ptr<Output> output, std::shared_ptr<Input> input) {
    output->disconnect(input);
}

void dsp::operator>>(Sample value, std::shared_ptr<dsp::InputParameter> input) {
    input->setDefaultValue(value);
}

void dsp::operator>>(Sample value, std::shared_ptr<dsp::OutputParameter> output) {
    output->setDefaultValue(value);
}

void dsp::operator>>(Array values, std::shared_ptr<InputParameter> input) {
    input->lock();
    if (values.size() > 0) {
        for (unsigned int i = 0; i < input->getNumChannels(); i++) {
            values[i % values.size()] >> input->getChannel(i);
        }
    }
    input->unlock();
}

void dsp::operator>>(Array values, std::shared_ptr<OutputParameter> output) {
    output->lock();
    if (values.size() > 0) {
        for (unsigned int i = 0; i < output->getNumChannels(); i++) {
            values[i % values.size()] >> output->getChannel(i);
        }
    }
    output->unlock();
}

void dsp::operator>>(std::shared_ptr<dsp::OutputParameter> output, std::shared_ptr<dsp::InputParameter> input) {
    input->connect(output);
}

void dsp::operator!=(std::shared_ptr<dsp::OutputParameter> output, std::shared_ptr<dsp::InputParameter> input) {
    output->disconnect(input);
}

void dsp::operator>>(std::shared_ptr<OutputParameter> output, std::shared_ptr<Input> input) {
    output->lock();
    for (unsigned int i = 0; i < output->getNumChannels(); i++) {
        output->getChannel(i) >> input;
    }
    output->unlock();
}

void dsp::operator!=(std::shared_ptr<OutputParameter> output, std::shared_ptr<Input> input) {
    output->lock();
    for (unsigned int i = 0; i < output->getNumChannels(); i++) {
        output->getChannel(i) != input;
    }
    output->unlock();
}

void dsp::operator>>(std::shared_ptr<Output> output, std::shared_ptr<InputParameter> input) {
    input->lock();
    for (unsigned int i = 0; i < input->getNumChannels(); i++) {
        output >> input->getChannel(i);
    }
    input->unlock();
}

void dsp::operator!=(std::shared_ptr<Output> output, std::shared_ptr<InputParameter> input) {
    input->lock();
    for (unsigned int i = 0; i < input->getNumChannels(); i++) {
        output != input->getChannel(i);
    }
    input->unlock();
}
