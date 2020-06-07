#include "Unit.h"

template <class T>
dsp::Unit::ConnectionParameter<T>::ConnectionParameter(unsigned int bufferSize,
                                                       Connection::Type type,
                                                       DSP_FLOAT value) {
    this->bufferSize = bufferSize;
    this->type = type;
    this->value = value;
}

template <class T> unsigned int dsp::Unit::ConnectionParameter<T>::getBufferSize() {
    return bufferSize;
}

template <class T> void dsp::Unit::ConnectionParameter<T>::setBufferSize(unsigned int bufferSize) {
    lock();
    for (const auto &channel : channels) {
        channel->setBufferSize(bufferSize);
    }
    this->bufferSize = bufferSize;
    unlock();
}

template <class T> dsp::Connection::Type dsp::Unit::ConnectionParameter<T>::getType() {
    return type;
}

template <class T> void dsp::Unit::ConnectionParameter<T>::setType(Connection::Type type) {
    lock();
    for (const auto &channel : channels) {
        channel->setType(type);
    }
    this->type = type;
    unlock();
}

template <class T> DSP_FLOAT dsp::Unit::ConnectionParameter<T>::getValue() {
    return value;
}

template <class T> void dsp::Unit::ConnectionParameter<T>::setValue(DSP_FLOAT value) {
    lock();
    for (const auto &channel : channels) {
        channel->setValue(value);
    }
    this->value = value;
    unlock();
}

template <class T> std::size_t dsp::Unit::ConnectionParameter<T>::getNumChannels() {
    return channels.size();
}

template <class T> void dsp::Unit::ConnectionParameter<T>::setNumChannels(std::size_t size) {
    lock();
    if (size < channels.size()) {
        channels.erase(channels.begin() + size, channels.end());
    } else {
        for (std::size_t i = channels.size(); i < size; i++) {
            channels.emplace_back(new T(bufferSize, type, value));
        }
    }
    unlock();
}

template <class T> std::vector<std::shared_ptr<T>> dsp::Unit::ConnectionParameter<T>::getChannels() {
    return channels;
}

template <class T> std::shared_ptr<T> dsp::Unit::ConnectionParameter<T>::getChannel(std::size_t index) {
    return channels[index];
}

dsp::Unit::InputParameter::InputParameter(unsigned int bufferSize, Connection::Type type, DSP_FLOAT value)
        : ConnectionParameter(bufferSize, type, value) {}

template class dsp::Unit::ConnectionParameter<dsp::Input>;

dsp::Unit::OutputParameter::OutputParameter(unsigned int bufferSize, Connection::Type type, DSP_FLOAT value)
        : ConnectionParameter(bufferSize, type, value) {}

template class dsp::Unit::ConnectionParameter<dsp::Output>;

dsp::Unit::Unit() : sampleRate(0), bufferSize(0) {}

unsigned int dsp::Unit::getSampleRate() {
    return sampleRate;
}

void dsp::Unit::setSampleRate(unsigned int sampleRate) {
    lock();
    for (const auto &unit : units) {
        unit->setSampleRate(sampleRate);
    }
    this->sampleRate = sampleRate;
    unlock();
}

unsigned int dsp::Unit::getBufferSize() {
    return bufferSize;
}

void dsp::Unit::setBufferSize(unsigned int bufferSize) {
    lock();
    for (const auto &unit : units) {
        unit->setBufferSize(bufferSize);
    }
    for (const auto &input : inputs) {
        input->setBufferSize(bufferSize);
    }
    for (const auto &output : outputs) {
        output->setBufferSize(bufferSize);
    }
    this->bufferSize = bufferSize;
    unlock();
}

std::size_t dsp::Unit::getNumInputs() {
    return inputs.size();
}

std::size_t dsp::Unit::getNumOutputs() {
    return outputs.size();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Unit::getInput(std::size_t index) {
    return inputs[index];
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::Unit::getOutput(std::size_t index) {
    return outputs[index];
}

void dsp::Unit::pushInput(std::shared_ptr<InputParameter> input) {
    lock();
    inputs.push_back(input);
    unlock();
}

void dsp::Unit::pushInput(Connection::Type type, DSP_FLOAT value) {
    lock();
    inputs.push_back(std::make_shared<InputParameter>(bufferSize, type, value));
    unlock();
}

void dsp::Unit::pushOutput(std::shared_ptr<OutputParameter> output) {
    lock();
    outputs.push_back(output);
    unlock();
}

void dsp::Unit::pushOutput(Connection::Type type, DSP_FLOAT value) {
    lock();
    outputs.push_back(std::make_shared<OutputParameter>(bufferSize, type, value));
    unlock();
}

void dsp::Unit::insertInput(std::size_t index, std::shared_ptr<InputParameter> input) {
    lock();
    inputs.insert(inputs.begin() + index, input);
    unlock();
}

void dsp::Unit::insertInput(std::size_t index, Connection::Type type, DSP_FLOAT value) {
    lock();
    inputs.insert(inputs.begin() + index, std::make_shared<InputParameter>(bufferSize, type, value));
    unlock();
}

void dsp::Unit::insertOutput(std::size_t index, std::shared_ptr<OutputParameter> output) {
    lock();
    outputs.insert(outputs.begin() + index, output);
    unlock();
}

void dsp::Unit::insertOutput(std::size_t index, Connection::Type type, DSP_FLOAT value) {
    lock();
    outputs.insert(outputs.begin() + index, std::make_shared<OutputParameter>(bufferSize, type, value));
    unlock();
}

void dsp::Unit::removeInput(std::shared_ptr<InputParameter> input) {
    lock();
    inputs.erase(std::remove(inputs.begin(), inputs.end(), input), inputs.end());
    unlock();
}

void dsp::Unit::removeInput(std::size_t index) {
    lock();
    inputs.erase(inputs.begin() + index);
    unlock();
}

void dsp::Unit::removeOutput(std::shared_ptr<OutputParameter> output) {
    lock();
    outputs.erase(std::remove(outputs.begin(), outputs.end(), output), outputs.end());
    unlock();
}

void dsp::Unit::removeOutput(std::size_t index) {
    lock();
    outputs.erase(outputs.begin() + index);
    unlock();
}

std::size_t dsp::Unit::getNumChannels() {
    return 0;
}

void dsp::Unit::setNumChannels(std::size_t size) {
    lock();
    for (const auto &input : inputs) {
        input->setNumChannels(size);
    }
    for (const auto &output : outputs) {
        output->setNumChannels(size);
    }
    unlock();
}

std::size_t dsp::Unit::getNumUnits() {
    return units.size();
}

std::shared_ptr<dsp::Unit> dsp::Unit::getUnit(std::size_t index) {
    return units[index];
}

void dsp::Unit::pushUnit(std::shared_ptr<Unit> unit) {
    lock();
    units.push_back(unit);
    unlock();
}

void dsp::Unit::insertUnit(std::size_t index, std::shared_ptr<Unit> unit) {
    lock();
    units.insert(units.begin() + index, unit);
    unlock();
}

void dsp::Unit::removeUnit(std::shared_ptr<Unit> unit) {
    lock();
    units.erase(std::remove(units.begin(), units.end(), unit), units.end());
    unlock();
}

void dsp::Unit::removeUnit(std::size_t index) {
    lock();
    units.erase(units.begin() + index);
    unlock();
}

void dsp::Unit::sortUnits() {
    lock();
    if (units.size() > 0) {
        std::unordered_set<Unit *> unitSet;
        std::unordered_map<Input *, Unit *> inputToUnit;
        std::unordered_map<Output *, Unit *> outputToUnit;
        for (const auto &unit : units) {
            unit->lock();
            unitSet.insert(unit.get());
            for (const auto &input : unit->inputs) {
                input->lock();
                for (const auto &channel : input->getChannels()) {
                    channel->lock();
                    inputToUnit[channel.get()] = unit.get();
                }
            }
            for (const auto &output : unit->outputs) {
                output->lock();
                for (const auto &channel : output->getChannels()) {
                    channel->lock();
                    outputToUnit[channel.get()] = unit.get();
                }
            }
        }
        std::queue<Unit *> queue;
        for (const auto &unit : units) {
            if ([&] {
                    for (const auto &input : unit->inputs) {
                        for (const auto &channel : input->getChannels()) {
                            for (const auto &output : channel->getConnections()) {
                                if (unitSet.find(outputToUnit[output]) != unitSet.end()) {
                                    return false;
                                }
                            }
                        }
                    }
                    return true;
                }()) {
                queue.push(unit.get());
            }
        }
        std::unordered_set<Unit *> explored;
        std::unordered_map<Unit *, std::size_t> orderMap;
        std::size_t increment = 0;
        while (!queue.empty()) {
            Unit *unit = queue.front();
            explored.insert(unit);
            for (const auto &output : unit->outputs) {
                for (const auto &channel : output->getChannels()) {
                    for (const auto &input : channel->getConnections()) {
                        Unit *child = inputToUnit[input];
                        if (unitSet.find(child) != unitSet.end() && explored.find(child) == explored.end()) {
                            queue.push(child);
                        }
                    }
                }
            }
            orderMap[unit] = increment;
            increment++;
            queue.pop();
        }
        for (const auto &unit : units) {
            for (const auto &output : unit->outputs) {
                for (const auto &channel : output->getChannels()) {
                    channel->unlock();
                }
                output->unlock();
            }
            for (const auto &input : unit->inputs) {
                for (const auto &channel : input->getChannels()) {
                    channel->unlock();
                }
                input->unlock();
            }
            unit->unlock();
        }
        std::sort(units.begin(), units.end(), [&orderMap](const auto &a, const auto &b) {
            return orderMap[a.get()] < orderMap[b.get()];
        });
    }
    unlock();
}

void dsp::Unit::run() {
    lock();
    for (const auto &unit : units) {
        unit->run();
    }
    process();
    unlock();
}

void dsp::Unit::connect() {}

void dsp::Unit::disconnect() {}

void dsp::Unit::process() {}

void dsp::operator>>(DSP_FLOAT value, dsp::Unit::InputParameter &input) {
    for (std::size_t i = 0; i < input.getNumChannels(); i++) {
        value >> *input.getChannel(i);
    }
}

void dsp::operator>>(dsp::Unit::OutputParameter &output, dsp::Unit::InputParameter &input) {
    for (std::size_t i = 0; i < input.getNumChannels(); i++) {
        *output.getChannel(i % output.getNumChannels()) >> *input.getChannel(i);
    }
}

void dsp::operator!=(dsp::Unit::OutputParameter &output, dsp::Unit::InputParameter &input) {
    for (std::size_t i = 0; i < input.getNumChannels(); i++) {
        *output.getChannel(i % output.getNumChannels()) != *input.getChannel(i);
    }
}
