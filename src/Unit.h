#pragma once

#include "Connection.h"
#include "Runnable.h"
#include <map>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace dsp {

class Unit : public Runnable {

public:
    template <class T> class ConnectionParameter : public Lockable {

    public:
        ConnectionParameter(unsigned int numChannels,
                            unsigned int bufferSize,
                            Type type,
                            Space space = Space::TIME,
                            DSP_FLOAT defaultValue = 0.0);

        unsigned int getNumChannels() const;
        void setNumChannels(unsigned int numChannels);

        unsigned int getBufferSize() const;
        void setBufferSize(unsigned int bufferSize);

        Type getType() const;
        void setType(Type type);

        Space getSpace() const;
        void setSpace(Space space);

        DSP_FLOAT getDefaultValue() const;
        void setDefaultValue(DSP_FLOAT defaultValue);

        std::vector<std::shared_ptr<T>> getChannels() const;
        std::shared_ptr<T> getChannel(unsigned int index) const;

    private:
        unsigned int bufferSize;
        Type type;
        Space space;
        DSP_FLOAT defaultValue;
        std::vector<std::shared_ptr<T>> channels;
    };

    class InputParameter : public ConnectionParameter<Input> {

    public:
        InputParameter(unsigned int numChannels,
                       unsigned int bufferSize,
                       Type type,
                       Space space = Space::TIME,
                       DSP_FLOAT value = 0.0);
    };

    class OutputParameter : public ConnectionParameter<Output> {

    public:
        OutputParameter(unsigned int numChannels,
                        unsigned int bufferSize,
                        Type type,
                        Space space = Space::TIME,
                        DSP_FLOAT value = 0.0);
    };

    Unit();

    unsigned int getNumChannels() const;
    void setNumChannels(unsigned int numChannels);

    unsigned int getNumInputs() const;
    unsigned int getNumOutputs() const;
    std::shared_ptr<InputParameter> getInput(unsigned int index) const;
    std::shared_ptr<OutputParameter> getOutput(unsigned int index) const;
    void pushInput(std::shared_ptr<InputParameter> input);
    void pushOutput(std::shared_ptr<OutputParameter> output);
    void pushInput(Type type, Space space = Space::TIME, DSP_FLOAT defaultValue = 0.0);
    void pushOutput(Type type, Space space = Space::TIME, DSP_FLOAT defaultValue = 0.0);
    void replaceInput(std::shared_ptr<InputParameter> input, std::shared_ptr<InputParameter> replacement);
    void replaceOutput(std::shared_ptr<OutputParameter> output, std::shared_ptr<OutputParameter> replacement);
    void removeInput(std::shared_ptr<InputParameter> input);
    void removeOutput(std::shared_ptr<OutputParameter> output);

    unsigned int getNumUnits() const;
    std::shared_ptr<Unit> getUnit(unsigned int index) const;
    void pushUnit(std::shared_ptr<Unit> unit, bool sort = false);
    void pushUnits(std::vector<std::shared_ptr<Unit>> units, bool sort = false);
    void replaceUnit(std::shared_ptr<Unit> unit, std::shared_ptr<Unit> replacement);
    void removeUnit(std::shared_ptr<Unit> unit);
    void removeUnits(std::vector<std::shared_ptr<Unit>> units);
    void sortUnits();

    void run();

protected:
    std::vector<std::shared_ptr<InputParameter>> inputs;
    std::vector<std::shared_ptr<OutputParameter>> outputs;
    std::vector<std::shared_ptr<Unit>> children;

    void setSampleRateNoLock(unsigned int sampleRate) override;
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    virtual void setNumChannelsNoLock(unsigned int numChannels);
    void sortUnitsNoLock();

    virtual void connect();
    virtual void disconnect();
    virtual void process();

private:
    unsigned int numChannels;
};

void operator>>(DSP_FLOAT value, std::shared_ptr<Unit::InputParameter> input);
void operator>>(DSP_FLOAT value, std::shared_ptr<Unit::OutputParameter> output);

void operator>>(std::vector<DSP_FLOAT> values, std::shared_ptr<Unit::InputParameter> input);
void operator>>(std::vector<DSP_FLOAT> values, std::shared_ptr<Unit::OutputParameter> output);

void operator>>(std::shared_ptr<Unit::OutputParameter> output, std::shared_ptr<Unit::InputParameter> input);
void operator!=(std::shared_ptr<Unit::OutputParameter> output, std::shared_ptr<Unit::InputParameter> input);

void operator>>(std::shared_ptr<Unit::OutputParameter> output, std::shared_ptr<Input> input);
void operator!=(std::shared_ptr<Unit::OutputParameter> output, std::shared_ptr<Input> input);

void operator>>(std::shared_ptr<Output> output, std::shared_ptr<Unit::InputParameter> input);
void operator!=(std::shared_ptr<Output> output, std::shared_ptr<Unit::InputParameter> input);

} // namespace dsp
