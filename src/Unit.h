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

        unsigned int getNumChannels();
        void setNumChannels(unsigned int numChannels);

        unsigned int getBufferSize();
        void setBufferSize(unsigned int bufferSize);

        Type getType();
        void setType(Type type);

        Space getSpace();
        void setSpace(Space space);

        DSP_FLOAT getDefaultValue();
        void setDefaultValue(DSP_FLOAT defaultValue);

        std::vector<std::shared_ptr<T>> getChannels();
        std::shared_ptr<T> getChannel(unsigned int index);

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

    unsigned int getNumChannels();
    void setNumChannels(unsigned int numChannels);

    unsigned int getNumInputs();
    unsigned int getNumOutputs();
    std::shared_ptr<InputParameter> getInput(unsigned int index);
    std::shared_ptr<OutputParameter> getOutput(unsigned int index);
    void setInput(unsigned int index, std::shared_ptr<InputParameter> input);
    void setOutput(unsigned int index, std::shared_ptr<OutputParameter> output);
    void pushInput(std::shared_ptr<InputParameter> input);
    void pushInput(Type type, Space space = Space::TIME, DSP_FLOAT defaultValue = 0.0);
    void pushOutput(std::shared_ptr<OutputParameter> output);
    void pushOutput(Type type, Space space = Space::TIME, DSP_FLOAT defaultValue = 0.0);
    void insertInput(unsigned int index, std::shared_ptr<InputParameter> input);
    void insertInput(unsigned int index, Type type, Space space = Space::TIME, DSP_FLOAT defaultValue = 0.0);
    void insertOutput(unsigned int index, std::shared_ptr<OutputParameter> output);
    void insertOutput(unsigned int index, Type type, Space space = Space::TIME, DSP_FLOAT defaultValue = 0.0);
    void removeInput(std::shared_ptr<InputParameter> input);
    void removeInput(unsigned int index);
    void removeOutput(std::shared_ptr<OutputParameter> output);
    void removeOutput(unsigned int index);

    unsigned int getNumUnits();
    std::shared_ptr<Unit> getUnit(unsigned int index);
    void pushUnit(std::shared_ptr<Unit> unit);
    void insertUnit(unsigned int index, std::shared_ptr<Unit> unit);
    void removeUnit(std::shared_ptr<Unit> unit);
    void removeUnit(unsigned int index);
    void sortUnits();

    void run();

protected:
    std::vector<std::shared_ptr<InputParameter>> inputs;
    std::vector<std::shared_ptr<OutputParameter>> outputs;
    std::vector<std::shared_ptr<Unit>> units;

    void setSampleRateNoLock(unsigned int sampleRate) override;
    void setBufferSizeNoLock(unsigned int bufferSize) override;
    virtual void setNumChannelsNoLock(unsigned int numChannels);

    virtual void connect();
    virtual void disconnect();
    virtual void process();

private:
    unsigned int numChannels;
};

void operator>>(DSP_FLOAT value, std::shared_ptr<Unit::InputParameter> input);
void operator>>(std::vector<DSP_FLOAT> values, std::shared_ptr<Unit::InputParameter> input);
void operator>>(std::shared_ptr<Unit::OutputParameter> output, std::shared_ptr<Unit::InputParameter> input);
void operator!=(std::shared_ptr<Unit::OutputParameter> output, std::shared_ptr<Unit::InputParameter> input);

} // namespace dsp
