#include "Connection.h"

dsp::Connection::Connection(unsigned int bufferSize, Type type, Space space, DSP_FLOAT value) {
    buffer.resize(bufferSize);
    this->type = type;
    this->space = space;
    this->value = value;
}

unsigned int dsp::Connection::getBufferSize() {
    return static_cast<unsigned int>(buffer.size());
}

void dsp::Connection::setBufferSize(unsigned int bufferSize) {
    buffer.resize(bufferSize);
}

std::vector<DSP_FLOAT> &dsp::Connection::getBuffer() {
    return buffer;
}

void dsp::Connection::fillBuffer(DSP_FLOAT value) {
    std::fill(buffer.begin(), buffer.end(), value);
}

dsp::Connection::Type dsp::Connection::getType() {
    return type;
}

void dsp::Connection::setType(Type type) {
    this->type = type;
}

dsp::Connection::Space dsp::Connection::getSpace() {
    return space;
}

void dsp::Connection::setSpace(Space space) {
    this->space = space;
}

DSP_FLOAT dsp::Connection::getValue() {
    return value;
}

void dsp::Connection::setValue(DSP_FLOAT value) {
    this->value = value;
}

dsp::Input::Input(unsigned int bufferSize, Type type, Space space, DSP_FLOAT value)
        : Connection(bufferSize, type, space, value) {}

dsp::Input::~Input() {
    disconnectAll();
}

std::vector<std::shared_ptr<dsp::Output>> dsp::Input::getConnections() {
    return connections;
}

void dsp::Input::connect(std::shared_ptr<Output> output) {
    lock();
    output->lock();

    output->addConnection(shared_from_this());
    addConnection(output);

    output->unlock();
    unlock();
}

void dsp::Input::disconnect(std::shared_ptr<Output> output) {
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
    connections.clear();
    for (const auto &output : connections) {
        output->unlock();
    }
    unlock();
}

void dsp::Input::addConnection(std::shared_ptr<Output> output) {
    if (std::find(connections.begin(), connections.end(), output) == connections.end()) {
        connections.push_back(output);
    }
}

void dsp::Input::removeConnection(std::shared_ptr<Output> output) {
    connections.erase(std::remove(connections.begin(), connections.end(), output), connections.end());
}

void dsp::Input::copyBuffers() {
    lock();
    if (connections.size() == 0) {
        fillBuffer(value);
    } else {
        fillBuffer(0.0);
    }
    for (const auto &output : connections) {
        std::transform(
                buffer.begin(), buffer.end(), output->getBuffer().begin(), buffer.begin(), std::plus<DSP_FLOAT>());
    }
    unlock();
}

dsp::Output::Output(unsigned int bufferSize, Type type, Space space, DSP_FLOAT value)
        : Connection(bufferSize, type, space, value) {}

dsp::Output::~Output() {
    disconnectAll();
}

std::vector<std::shared_ptr<dsp::Input>> dsp::Output::getConnections() {
    return connections;
}

void dsp::Output::connect(std::shared_ptr<Input> input) {
    lock();
    input->lock();

    input->addConnection(shared_from_this());
    addConnection(input);

    input->unlock();
    unlock();
}

void dsp::Output::disconnect(std::shared_ptr<Input> input) {
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
    connections.clear();
    for (const auto &input : connections) {
        input->unlock();
    }
    unlock();
}

void dsp::Output::addConnection(std::shared_ptr<Input> input) {
    connections.push_back(input);
}

void dsp::Output::removeConnection(std::shared_ptr<Input> input) {
    connections.erase(std::remove(connections.begin(), connections.end(), input), connections.end());
}

void dsp::operator>>(DSP_FLOAT value, std::shared_ptr<Input> input) {
    input->setValue(value);
}

void dsp::operator>>(std::shared_ptr<Output> output, std::shared_ptr<Input> input) {
    input->connect(output);
}

void dsp::operator!=(std::shared_ptr<Output> output, std::shared_ptr<Input> input) {
    output->disconnect(input);
}
