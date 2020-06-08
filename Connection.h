#pragma once

#include "Constants.h"
#include "Functions.h"
#include "Lockable.h"
#include <string>
#include <vector>

namespace dsp {

class Unit;
class Input;
class Output;

class Connection {

public:
    enum class Type { BIPOLAR, UNIPOLAR, SECONDS, HERTZ, DECIBELS, BINARY, INTEGER, RATIO, FFT_REAL, FFT_IMAG };

    Connection(unsigned int bufferSize, Type type, DSP_FLOAT value);

    unsigned int getBufferSize();
    void setBufferSize(unsigned int bufferSize);
    std::vector<DSP_FLOAT> &getBuffer();
    void fillBuffer(DSP_FLOAT value);

    Type getType();
    void setType(Type type);

    DSP_FLOAT getValue();
    void setValue(DSP_FLOAT value);

protected:
    std::vector<DSP_FLOAT> buffer;
    Type type;
    DSP_FLOAT value;
};

class Input : public Connection, public Lockable, public std::enable_shared_from_this<Input> {
    friend class Output;

public:
    Input(unsigned int bufferSize, Type type, DSP_FLOAT value);
    ~Input();
    std::vector<std::shared_ptr<Output>> getConnections();
    void connect(std::shared_ptr<Output> output);
    void disconnect(std::shared_ptr<Output> output);
    void disconnectAll();
    void copyBuffers();

private:
    std::vector<std::shared_ptr<Output>> connections;
    void addConnection(std::shared_ptr<Output> output);
    void removeConnection(std::shared_ptr<Output> output);
};

class Output : public Connection, public Lockable, public std::enable_shared_from_this<Output> {
    friend class Input;

public:
    Output(unsigned int bufferSize, Type type, DSP_FLOAT value);
    ~Output();
    std::vector<std::shared_ptr<Input>> getConnections();
    void connect(std::shared_ptr<Input> input);
    void disconnect(std::shared_ptr<Input> input);
    void disconnectAll();

private:
    std::vector<std::shared_ptr<Input>> connections;
    void addConnection(std::shared_ptr<Input> input);
    void removeConnection(std::shared_ptr<Input> input);
};

void operator>>(DSP_FLOAT value, std::shared_ptr<Input> input);
void operator>>(std::shared_ptr<Output> output, std::shared_ptr<Input> input);
void operator!=(std::shared_ptr<Output> output, std::shared_ptr<Input> input);

} // namespace dsp
