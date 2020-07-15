#include "Unit.h"

dsp::Unit::Unit()
        : numChannels(0)
        , active(true) {}

unsigned int dsp::Unit::getNumChannels() const {
    return numChannels;
}

void dsp::Unit::setNumChannels(unsigned int numChannels) {
    lock();
    setNumChannelsNoLock(numChannels);
    unlock();
}

bool dsp::Unit::isActive() const {
    return active;
}

void dsp::Unit::setActive(bool active) {
    lock();
    this->active = active;
    unlock();
}

unsigned int dsp::Unit::getNumInputs() const {
    return static_cast<unsigned int>(inputs.size());
}

unsigned int dsp::Unit::getNumOutputs() const {
    return static_cast<unsigned int>(outputs.size());
}

std::shared_ptr<dsp::InputParameter> dsp::Unit::getInput(unsigned int index) const {
    assert(index < inputs.size());
    return inputs[index];
}

std::shared_ptr<dsp::OutputParameter> dsp::Unit::getOutput(unsigned int index) const {
    assert(index < outputs.size());
    return outputs[index];
}

void dsp::Unit::pushInput(std::shared_ptr<InputParameter> input) {
    assert(input != nullptr);
    lock();
    input->setBufferSize(getBufferSize());
    input->setNumChannels(getNumChannels());
    inputs.push_back(input);
    unlock();
}

void dsp::Unit::pushOutput(std::shared_ptr<OutputParameter> output) {
    assert(output != nullptr);
    lock();
    output->setBufferSize(getBufferSize());
    output->setNumChannels(getNumChannels());
    outputs.push_back(output);
    unlock();
}

std::shared_ptr<dsp::InputParameter> dsp::Unit::pushInput(Type type, Space space, DSP_FLOAT value) {
    auto input = std::make_shared<InputParameter>(getNumChannels(), getBufferSize(), type, space, value);
    lock();
    inputs.push_back(input);
    unlock();
    return input;
}

std::shared_ptr<dsp::OutputParameter> dsp::Unit::pushOutput(Type type, Space space, DSP_FLOAT value) {
    auto output = std::make_shared<OutputParameter>(getNumChannels(), getBufferSize(), type, space, value);
    lock();
    outputs.push_back(output);
    unlock();
    return output;
}

void dsp::Unit::replaceInput(std::shared_ptr<InputParameter> input, std::shared_ptr<InputParameter> replacement) {
    assert(replacement != nullptr);
    lock();
    std::replace(inputs.begin(), inputs.end(), input, replacement);
    unlock();
}

void dsp::Unit::replaceOutput(std::shared_ptr<OutputParameter> output, std::shared_ptr<OutputParameter> replacement) {
    assert(replacement != nullptr);
    lock();
    std::replace(outputs.begin(), outputs.end(), output, replacement);
    unlock();
}

void dsp::Unit::removeInput(std::shared_ptr<InputParameter> input) {
    lock();
    inputs.erase(std::remove(inputs.begin(), inputs.end(), input), inputs.end());
    unlock();
}

void dsp::Unit::removeOutput(std::shared_ptr<OutputParameter> output) {
    lock();
    outputs.erase(std::remove(outputs.begin(), outputs.end(), output), outputs.end());
    unlock();
}

unsigned int dsp::Unit::getNumUnits() const {
    return static_cast<unsigned int>(children.size());
}

std::shared_ptr<dsp::Unit> dsp::Unit::getUnit(unsigned int index) const {
    assert(index < children.size());
    return children[index];
}

void dsp::Unit::pushUnit(std::shared_ptr<Unit> unit, bool sort) {
    assert(unit != nullptr);
    lock();
    unit->setSampleRate(getSampleRate());
    unit->setBufferSize(getBufferSize());
    if (getNumChannels() > 0) {
        unit->setNumChannels(getNumChannels());
    }
    children.push_back(unit);
    if (sort) {
        sortUnitsNoLock();
    }
    unlock();
}

void dsp::Unit::pushUnits(std::vector<std::shared_ptr<Unit>> units, bool sort) {
    for (const auto &unit : units) {
        assert(unit != nullptr);
    }
    lock();
    for (const auto &unit : units) {
        unit->setSampleRate(getSampleRate());
        unit->setBufferSize(getBufferSize());
        if (getNumChannels() > 0) {
            unit->setNumChannels(getNumChannels());
        }
        children.push_back(unit);
    }
    if (sort) {
        sortUnitsNoLock();
    }
    unlock();
}

void dsp::Unit::replaceUnit(std::shared_ptr<Unit> unit, std::shared_ptr<Unit> replacement) {
    assert(replacement != nullptr);
    lock();
    std::replace(children.begin(), children.end(), unit, replacement);
    unlock();
}

void dsp::Unit::removeUnit(std::shared_ptr<Unit> unit) {
    lock();
    children.erase(std::remove(children.begin(), children.end(), unit), children.end());
    unlock();
}

void dsp::Unit::removeUnits(std::vector<std::shared_ptr<Unit>> units) {
    lock();
    for (const auto &unit : units) {
        children.erase(std::remove(children.begin(), children.end(), unit), children.end());
    }
    unlock();
}

void dsp::Unit::sortUnits() {
    lock();
    sortUnitsNoLock();
    unlock();
}

void dsp::Unit::run() {
    lock();
    prepare();
    if (active) {
        if (children.size() == 0) {
            process();
        } else {
            for (const auto &unit : children) {
                unit->run();
            }
        }
    }
    unlock();
}

void dsp::Unit::setSampleRateNoLock(unsigned int sampleRate) {
    Runnable::setSampleRateNoLock(sampleRate);
    for (const auto &unit : children) {
        unit->setSampleRate(sampleRate);
    }
}

void dsp::Unit::setBufferSizeNoLock(unsigned int bufferSize) {
    Runnable::setBufferSizeNoLock(bufferSize);
    for (const auto &input : inputs) {
        input->setBufferSize(bufferSize);
    }
    for (const auto &output : outputs) {
        output->setBufferSize(bufferSize);
    }
    for (const auto &unit : children) {
        unit->setBufferSize(bufferSize);
    }
}

void dsp::Unit::setNumChannelsNoLock(unsigned int numChannels) {
    this->numChannels = numChannels;
    for (const auto &input : inputs) {
        input->setNumChannels(numChannels);
    }
    for (const auto &output : outputs) {
        output->setNumChannels(numChannels);
    }
    if (numChannels > 0) {
        disconnect();
        for (const auto &unit : children) {
            unit->setNumChannels(numChannels);
        }
        connect();
    }
}

void dsp::Unit::sortUnitsNoLock() {
    if (children.size() > 0) {
        std::unordered_set<Unit *> unitSet;
        std::unordered_map<Input *, Unit *> inputToUnit;
        std::unordered_map<Output *, Unit *> outputToUnit;
        for (const auto &unit : children) {
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
        for (const auto &unit : children) {
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
        for (const auto &unit : children) {
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
        std::sort(children.begin(), children.end(), [&orderMap](const auto &a, const auto &b) {
            return orderMap[a.get()] < orderMap[b.get()];
        });
    }
}

void dsp::Unit::connect() {}

void dsp::Unit::disconnect() {}

void dsp::Unit::process() {}

void dsp::Unit::prepare() {
    for (const auto &input : inputs) {
        input->copyBuffers();
    }
    for (const auto &output : outputs) {
        output->clearBuffer();
    }
}
