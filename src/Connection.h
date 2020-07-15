#pragma once

#include "Constants.h"
#include "Functions.h"
#include "Lockable.h"
#include <string>
#include <vector>

namespace dsp {

class Input;
class Output;
class InputParameter;
class OutputParameter;

class Connection : public Lockable {

public:
    Connection(unsigned int bufferSize, Type type, Space space = Space::TIME, DSP_FLOAT defaultValue = 0.0);

    unsigned int getBufferSize() const;
    void setBufferSize(unsigned int bufferSize);

    Type getType() const;
    void setType(Type type);

    Space getSpace() const;
    void setSpace(Space space);

    DSP_FLOAT getDefaultValue() const;
    void setDefaultValue(DSP_FLOAT defaultValue);

    void fillBuffer(DSP_FLOAT value);
    void clearBuffer();

    std::vector<DSP_FLOAT> &getBuffer();

protected:
    std::vector<DSP_FLOAT> buffer;
    Type type;
    Space space;
    DSP_FLOAT defaultValue;
};

class Input : public Connection, public std::enable_shared_from_this<Input> {
    friend class Output;

public:
    Input(unsigned int bufferSize, Type type, Space space = Space::TIME, DSP_FLOAT value = 0.0);
    ~Input();

    std::vector<std::shared_ptr<Output>> getConnections() const;
    void connect(std::shared_ptr<Output> output);
    void disconnect(std::shared_ptr<Output> output);
    void disconnectAll();
    void copyBuffers();

private:
    std::vector<std::shared_ptr<Output>> connections;

    void addConnection(std::shared_ptr<Output> output);
    void removeConnection(std::shared_ptr<Output> output);
};

class Output : public Connection, public std::enable_shared_from_this<Output> {
    friend class Input;

public:
    Output(unsigned int bufferSize, Type type, Space space = Space::TIME, DSP_FLOAT value = 0.0);
    ~Output();

    std::vector<std::shared_ptr<Input>> getConnections() const;
    void connect(std::shared_ptr<Input> input);
    void disconnect(std::shared_ptr<Input> input);
    void disconnectAll();

private:
    std::vector<std::shared_ptr<Input>> connections;

    void addConnection(std::shared_ptr<Input> input);
    void removeConnection(std::shared_ptr<Input> input);
};

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

    void fillBuffer(DSP_FLOAT value);
    void clearBuffer();

    std::vector<std::shared_ptr<T>> getChannels() const;
    std::shared_ptr<T> getChannel(unsigned int channel) const;

protected:
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

    std::vector<std::shared_ptr<Output>> getConnections();
    void connect(std::shared_ptr<OutputParameter> output);
    void disconnect(std::shared_ptr<OutputParameter> output);
    void disconnectAll();
    void copyBuffers();
};

class OutputParameter : public ConnectionParameter<Output> {

public:
    OutputParameter(unsigned int numChannels,
                    unsigned int bufferSize,
                    Type type,
                    Space space = Space::TIME,
                    DSP_FLOAT value = 0.0);

    std::vector<std::shared_ptr<Input>> getConnections();
    void connect(std::shared_ptr<InputParameter> input);
    void disconnect(std::shared_ptr<InputParameter> input);
    void disconnectAll();
};

void operator>>(DSP_FLOAT value, std::shared_ptr<Input> input);
void operator>>(DSP_FLOAT value, std::shared_ptr<Output> output);

void operator>>(std::shared_ptr<Output> output, std::shared_ptr<Input> input);
void operator!=(std::shared_ptr<Output> output, std::shared_ptr<Input> input);

void operator>>(DSP_FLOAT value, std::shared_ptr<InputParameter> input);
void operator>>(DSP_FLOAT value, std::shared_ptr<OutputParameter> output);

void operator>>(std::vector<DSP_FLOAT> values, std::shared_ptr<InputParameter> input);
void operator>>(std::vector<DSP_FLOAT> values, std::shared_ptr<OutputParameter> output);

void operator>>(std::shared_ptr<OutputParameter> output, std::shared_ptr<InputParameter> input);
void operator!=(std::shared_ptr<OutputParameter> output, std::shared_ptr<InputParameter> input);

void operator>>(std::shared_ptr<OutputParameter> output, std::shared_ptr<Input> input);
void operator!=(std::shared_ptr<OutputParameter> output, std::shared_ptr<Input> input);

void operator>>(std::shared_ptr<Output> output, std::shared_ptr<InputParameter> input);
void operator!=(std::shared_ptr<Output> output, std::shared_ptr<InputParameter> input);

} // namespace dsp
