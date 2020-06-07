#pragma once

#include "Connection.h"
#include <map>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace dsp {

class Unit : public Lockable {

public:
    template <class T> class ConnectionParameter : public Lockable {

    public:
        ConnectionParameter(unsigned int bufferSize, Connection::Type type, DSP_FLOAT value);

        unsigned int getBufferSize();
        void setBufferSize(unsigned int bufferSize);

        Connection::Type getType();
        void setType(Connection::Type type);

        DSP_FLOAT getValue();
        void setValue(DSP_FLOAT value);

        std::size_t getNumChannels();
        void setNumChannels(std::size_t size);
        std::vector<std::shared_ptr<T>> getChannels();
        std::shared_ptr<T> getChannel(std::size_t index);

    private:
        unsigned int bufferSize;
        Connection::Type type;
        DSP_FLOAT value;
        std::vector<std::shared_ptr<T>> channels;
    };

    class InputParameter : public ConnectionParameter<Input> {

    public:
        InputParameter(unsigned int bufferSize, Connection::Type type, DSP_FLOAT value);
    };

    class OutputParameter : public ConnectionParameter<Output> {

    public:
        OutputParameter(unsigned int bufferSize, Connection::Type type, DSP_FLOAT value);
    };

    Unit();

    unsigned int getSampleRate();
    virtual void setSampleRate(unsigned int sampleRate);

    unsigned int getBufferSize();
    virtual void setBufferSize(unsigned int bufferSize);

    std::size_t getNumInputs();
    std::size_t getNumOutputs();
    std::shared_ptr<InputParameter> getInput(std::size_t index);
    std::shared_ptr<OutputParameter> getOutput(std::size_t index);
    void pushInput(std::shared_ptr<InputParameter> input);
    void pushInput(Connection::Type type, DSP_FLOAT value = 0);
    void pushOutput(std::shared_ptr<OutputParameter> output);
    void pushOutput(Connection::Type type, DSP_FLOAT value = 0);
    void insertInput(std::size_t index, Connection::Type type, DSP_FLOAT value = 0);
    void insertOutput(std::size_t index, Connection::Type type, DSP_FLOAT value = 0);
    void removeInput(std::size_t index);
    void removeOutput(std::size_t index);

    virtual std::size_t getNumChannels();
    virtual void setNumChannels(std::size_t size);

    std::size_t getNumUnits();
    std::shared_ptr<Unit> getUnit(std::size_t index);
    void pushUnit(Unit *unit);
    void insertUnit(std::size_t index, Unit *unit);
    void removeUnit(std::size_t index);
    void sortUnits();

    void run();

protected:
    std::vector<std::shared_ptr<InputParameter>> inputs;
    std::vector<std::shared_ptr<OutputParameter>> outputs;
    std::vector<std::shared_ptr<Unit>> units;
    virtual void connect();
    virtual void disconnect();
    virtual void process();

private:
    unsigned int sampleRate;
    unsigned int bufferSize;
};

void operator>>(DSP_FLOAT value, Unit::InputParameter &input);
void operator>>(Unit::OutputParameter &output, Unit::InputParameter &input);
void operator!=(Unit::OutputParameter &output, Unit::InputParameter &input);

} // namespace dsp
