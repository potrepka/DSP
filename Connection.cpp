#include "Connection.h"

dsp::Connection::Connection(unsigned int bufferSize, Type type, DSP_FLOAT value) {
    buffer.resize(bufferSize);
    this->type = type;
    this->value = value;
}

unsigned int dsp::Connection::getBufferSize() {
    return static_cast<unsigned int>(buffer.size());
}

void dsp::Connection::setBufferSize(unsigned int bufferSize) {
    buffer.resize(bufferSize);
}

std::vector<DSP_FLOAT>& dsp::Connection::getBuffer() {
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

DSP_FLOAT dsp::Connection::getValue() {
    return value;
}

void dsp::Connection::setValue(DSP_FLOAT value) {
    this->value = value;
}

dsp::Input::Input(unsigned int bufferSize, Type type, DSP_FLOAT value) : Connection(bufferSize, type, value) {}

dsp::Input::~Input() {
    disconnectAll();
}

std::vector<dsp::Output*> dsp::Input::getConnections() {
    return connections;
}

void dsp::Input::connect(Output* output) {
    lock();
    output->lock();

    output->addConnection(this);
    addConnection(output);

    output->unlock();
    unlock();
}

void dsp::Input::disconnect(Output* output) {
    lock();
    output->lock();

    output->removeConnection(this);
    removeConnection(output);

    output->unlock();
    unlock();
}

void dsp::Input::disconnectAll() {
    lock();
    for (Output* output : connections) {
        output->lock();
    }
    for (Output* output : connections) {
        output->removeConnection(this);
    }
    connections.clear();
    for (Output* output : connections) {
        output->unlock();
    }
    unlock();
}

void dsp::Input::addConnection(Output* output) {
    if (std::find(connections.begin(), connections.end(), output) == connections.end()) {
        connections.push_back(output);
    }
}

void dsp::Input::removeConnection(Output* output) {
    connections.erase(std::remove(connections.begin(), connections.end(), output), connections.end());
}

void dsp::Input::copyBuffers() {
    fillBuffer(value);
    lock();
    for (Output* output : connections) {
        std::transform(buffer.begin(),
                       buffer.end(),
                       output->getBuffer().begin(),
                       buffer.begin(),
                       std::plus<DSP_FLOAT>());
    }
    unlock();
}

dsp::Output::Output(unsigned int bufferSize, Type type, DSP_FLOAT value) : Connection(bufferSize, type, value) {}

dsp::Output::~Output() {
    disconnectAll();
}

std::vector<dsp::Input*> dsp::Output::getConnections() {
    return connections;
}

void dsp::Output::connect(Input* input) {
    lock();
    input->lock();

    input->addConnection(this);
    addConnection(input);

    input->unlock();
    unlock();
}

void dsp::Output::disconnect(Input* input) {
    lock();
    input->lock();

    input->removeConnection(this);
    removeConnection(input);

    input->unlock();
    unlock();
}

void dsp::Output::disconnectAll() {
    lock();
    for (Input* input : connections) {
        input->lock();
    }
    for (Input* input : connections) {
        input->removeConnection(this);
    }
    connections.clear();
    for (Input* input : connections) {
        input->unlock();
    }
    unlock();
}

void dsp::Output::addConnection(Input* input) {
    connections.push_back(input);
}

void dsp::Output::removeConnection(Input* input) {
    connections.erase(std::remove(connections.begin(), connections.end(), input), connections.end());
}

void dsp::operator>>(DSP_FLOAT value, Input& input) {
    input.setValue(value);
}

void dsp::operator>>(Output& output, Input& input) {
    input.connect(&output);
}

void dsp::operator!=(Output& output, Input& input) {
    output.disconnect(&input);
}
