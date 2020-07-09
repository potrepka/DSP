#include "Unit.h"

template <class T>
dsp::Unit::ConnectionParameter<T>::ConnectionParameter(
        unsigned int numChannels, unsigned int bufferSize, Type type, Space space, DSP_FLOAT defaultValue)
        : bufferSize(bufferSize), type(type), space(space), defaultValue(defaultValue) {
    setNumChannels(numChannels);
}

template <class T> unsigned int dsp::Unit::ConnectionParameter<T>::getNumChannels() {
    return static_cast<unsigned int>(channels.size());
}

template <class T> void dsp::Unit::ConnectionParameter<T>::setNumChannels(unsigned int numChannels) {
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

template <class T> unsigned int dsp::Unit::ConnectionParameter<T>::getBufferSize() {
    return bufferSize;
}

template <class T> void dsp::Unit::ConnectionParameter<T>::setBufferSize(unsigned int bufferSize) {
    lock();
    this->bufferSize = bufferSize;
    for (const auto &channel : channels) {
        channel->setBufferSize(bufferSize);
    }
    unlock();
}

template <class T> dsp::Type dsp::Unit::ConnectionParameter<T>::getType() {
    return type;
}

template <class T> void dsp::Unit::ConnectionParameter<T>::setType(Type type) {
    lock();
    this->type = type;
    for (const auto &channel : channels) {
        channel->setType(type);
    }
    unlock();
}

template <class T> dsp::Space dsp::Unit::ConnectionParameter<T>::getSpace() {
    return space;
}

template <class T> void dsp::Unit::ConnectionParameter<T>::setSpace(Space space) {
    lock();
    this->space = space;
    for (const auto &channel : channels) {
        channel->setSpace(space);
    }
    unlock();
}

template <class T> DSP_FLOAT dsp::Unit::ConnectionParameter<T>::getDefaultValue() {
    return defaultValue;
}

template <class T> void dsp::Unit::ConnectionParameter<T>::setDefaultValue(DSP_FLOAT defaultValue) {
    lock();
    this->defaultValue = defaultValue;
    for (const auto &channel : channels) {
        channel->setDefaultValue(defaultValue);
    }
    unlock();
}

template <class T> std::vector<std::shared_ptr<T>> dsp::Unit::ConnectionParameter<T>::getChannels() {
    return channels;
}

template <class T> std::shared_ptr<T> dsp::Unit::ConnectionParameter<T>::getChannel(unsigned int index) {
    return channels[index];
}

template class dsp::Unit::ConnectionParameter<dsp::Input>;

dsp::Unit::InputParameter::InputParameter(
        unsigned int numChannels, unsigned int bufferSize, Type type, Space space, DSP_FLOAT defaultValue)
        : ConnectionParameter(numChannels, bufferSize, type, space, defaultValue) {}

template class dsp::Unit::ConnectionParameter<dsp::Output>;

dsp::Unit::OutputParameter::OutputParameter(
        unsigned int numChannels, unsigned int bufferSize, Type type, Space space, DSP_FLOAT defaultValue)
        : ConnectionParameter(numChannels, bufferSize, type, space, defaultValue) {}

dsp::Unit::Unit() : numChannels(0) {}

unsigned int dsp::Unit::getNumChannels() {
    return numChannels;
}

void dsp::Unit::setNumChannels(unsigned int numChannels) {
    lock();
    setNumChannelsNoLock(numChannels);
    unlock();
}

unsigned int dsp::Unit::getNumInputs() {
    return static_cast<unsigned int>(inputs.size());
}

unsigned int dsp::Unit::getNumOutputs() {
    return static_cast<unsigned int>(outputs.size());
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::Unit::getInput(unsigned int index) {
    return inputs[index];
}

std::shared_ptr<dsp::Unit::OutputParameter> dsp::Unit::getOutput(unsigned int index) {
    return outputs[index];
}

void dsp::Unit::setInput(unsigned int index, std::shared_ptr<InputParameter> input) {
    inputs[index] = input;
}

void dsp::Unit::setOutput(unsigned int index, std::shared_ptr<dsp::Unit::OutputParameter> output) {
    outputs[index] = output;
}

void dsp::Unit::pushInput(std::shared_ptr<InputParameter> input) {
    lock();
    input->setBufferSize(getBufferSize());
    input->setNumChannels(getNumChannels());
    inputs.push_back(input);
    unlock();
}

void dsp::Unit::pushInput(Type type, Space space, DSP_FLOAT value) {
    lock();
    inputs.push_back(std::make_shared<InputParameter>(getNumChannels(), getBufferSize(), type, space, value));
    unlock();
}

void dsp::Unit::pushOutput(std::shared_ptr<OutputParameter> output) {
    lock();
    output->setBufferSize(getBufferSize());
    output->setNumChannels(getNumChannels());
    outputs.push_back(output);
    unlock();
}

void dsp::Unit::pushOutput(Type type, Space space, DSP_FLOAT value) {
    lock();
    outputs.push_back(std::make_shared<OutputParameter>(getNumChannels(), getBufferSize(), type, space, value));
    unlock();
}

void dsp::Unit::insertInput(unsigned int index, std::shared_ptr<InputParameter> input) {
    lock();
    input->setBufferSize(getBufferSize());
    input->setNumChannels(getNumChannels());
    inputs.insert(inputs.begin() + index, input);
    unlock();
}

void dsp::Unit::insertInput(unsigned int index, Type type, Space space, DSP_FLOAT value) {
    lock();
    inputs.insert(inputs.begin() + index,
                  std::make_shared<InputParameter>(getNumChannels(), getBufferSize(), type, space, value));
    unlock();
}

void dsp::Unit::insertOutput(unsigned int index, std::shared_ptr<OutputParameter> output) {
    lock();
    output->setBufferSize(getBufferSize());
    output->setNumChannels(getNumChannels());
    outputs.insert(outputs.begin() + index, output);
    unlock();
}

void dsp::Unit::insertOutput(unsigned int index, Type type, Space space, DSP_FLOAT value) {
    lock();
    outputs.insert(outputs.begin() + index,
                   std::make_shared<OutputParameter>(getNumChannels(), getBufferSize(), type, space, value));
    unlock();
}

void dsp::Unit::removeInput(std::shared_ptr<InputParameter> input) {
    lock();
    inputs.erase(std::remove(inputs.begin(), inputs.end(), input), inputs.end());
    unlock();
}

void dsp::Unit::removeInput(unsigned int index) {
    lock();
    inputs.erase(inputs.begin() + index);
    unlock();
}

void dsp::Unit::removeOutput(std::shared_ptr<OutputParameter> output) {
    lock();
    outputs.erase(std::remove(outputs.begin(), outputs.end(), output), outputs.end());
    unlock();
}

void dsp::Unit::removeOutput(unsigned int index) {
    lock();
    outputs.erase(outputs.begin() + index);
    unlock();
}

unsigned int dsp::Unit::getNumUnits() {
    return static_cast<unsigned int>(units.size());
}

std::shared_ptr<dsp::Unit> dsp::Unit::getUnit(unsigned int index) {
    return units[index];
}

void dsp::Unit::pushUnit(std::shared_ptr<Unit> unit) {
    lock();
    unit->setSampleRate(getSampleRate());
    unit->setBufferSize(getBufferSize());
    if (getNumChannels() > 0) {
        unit->setNumChannels(getNumChannels());
    }
    units.push_back(unit);
    unlock();
}

void dsp::Unit::insertUnit(unsigned int index, std::shared_ptr<Unit> unit) {
    lock();
    unit->setSampleRate(getSampleRate());
    unit->setBufferSize(getBufferSize());
    if (getNumChannels() > 0) {
        unit->setNumChannels(getNumChannels());
    }
    units.insert(units.begin() + index, unit);
    unlock();
}

void dsp::Unit::removeUnit(std::shared_ptr<Unit> unit) {
    lock();
    units.erase(std::remove(units.begin(), units.end(), unit), units.end());
    unlock();
}

void dsp::Unit::removeUnit(unsigned int index) {
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
                                if (unitSet.find(outputToUnit[output.get()]) != unitSet.end()) {
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
        std::unordered_map<Unit *, unsigned int> orderMap;
        unsigned int increment = 0;
        while (!queue.empty()) {
            Unit *unit = queue.front();
            explored.insert(unit);
            for (const auto &output : unit->outputs) {
                for (const auto &channel : output->getChannels()) {
                    for (const auto &input : channel->getConnections()) {
                        Unit *child = inputToUnit[input.get()];
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
    if (units.size() > 0) {
        for (const auto &unit : units) {
            unit->run();
        }
    } else {
        process();
    }
    unlock();
}

void dsp::Unit::setSampleRateNoLock(unsigned int sampleRate) {
    Runnable::setSampleRateNoLock(sampleRate);
    for (const auto &unit : units) {
        unit->setSampleRate(sampleRate);
    }
}

void dsp::Unit::setBufferSizeNoLock(unsigned int bufferSize) {
    Runnable::setBufferSizeNoLock(bufferSize);
    for (const auto &unit : units) {
        unit->setBufferSize(bufferSize);
    }
    for (const auto &input : inputs) {
        input->setBufferSize(bufferSize);
    }
    for (const auto &output : outputs) {
        output->setBufferSize(bufferSize);
    }
}

void dsp::Unit::setNumChannelsNoLock(unsigned int numChannels) {
    this->numChannels = numChannels;
    if (units.size() > 0) {
        if (numChannels > 0) {
            disconnect();
            for (const auto &unit : units) {
                unit->setNumChannels(numChannels);
            }
            connect();
        }
    } else {
        for (const auto &input : inputs) {
            input->setNumChannels(numChannels);
        }
        for (const auto &output : outputs) {
            output->setNumChannels(numChannels);
        }
    }
}

void dsp::Unit::connect() {}

void dsp::Unit::disconnect() {}

void dsp::Unit::process() {
    for (const auto &input : inputs) {
        input->lock();
        for (const auto &channel : input->getChannels()) {
            channel->copyBuffers();
        }
        input->unlock();
    }
    for (const auto &output : outputs) {
        output->lock();
        for (const auto &channel : output->getChannels()) {
            channel->fillBuffer(channel->getDefaultValue());
        }
        output->unlock();
    }
}

void dsp::operator>>(DSP_FLOAT value, std::shared_ptr<dsp::Unit::InputParameter> input) {
    input->lock();
    for (unsigned int i = 0; i < input->getNumChannels(); i++) {
        value >> input->getChannel(i);
    }
    input->unlock();
}

void dsp::operator>>(std::vector<DSP_FLOAT> values, std::shared_ptr<Unit::InputParameter> input) {
    input->lock();
    if (values.size() > 0) {
        for (unsigned int i = 0; i < input->getNumChannels(); i++) {
            values[i % values.size()] >> input->getChannel(i);
        }
    }
    input->unlock();
}

void dsp::operator>>(std::shared_ptr<dsp::Unit::OutputParameter> output,
                     std::shared_ptr<dsp::Unit::InputParameter> input) {
    input->lock();
    output->lock();
    if (output->getNumChannels() > 0) {
        for (unsigned int i = 0; i < input->getNumChannels(); i++) {
            output->getChannel(i % output->getNumChannels()) >> input->getChannel(i);
        }
    }
    output->unlock();
    input->unlock();
}

void dsp::operator!=(std::shared_ptr<dsp::Unit::OutputParameter> output,
                     std::shared_ptr<dsp::Unit::InputParameter> input) {
    output->lock();
    input->lock();
    if (output->getNumChannels() > 0) {
        for (unsigned int i = 0; i < input->getNumChannels(); i++) {
            output->getChannel(i % output->getNumChannels()) != input->getChannel(i);
        }
    }
    input->unlock();
    output->unlock();
}
