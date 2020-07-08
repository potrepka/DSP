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

class Connection : public Lockable {

public:
    Connection(unsigned int bufferSize, Type type, Space space = Space::TIME, DSP_FLOAT defaultValue = 0.0);

    unsigned int getBufferSize();
    void setBufferSize(unsigned int bufferSize);

    Type getType();
    void setType(Type type);

    Space getSpace();
    void setSpace(Space space);

    DSP_FLOAT getDefaultValue();
    void setDefaultValue(DSP_FLOAT defaultValue);

    std::vector<DSP_FLOAT> &getBuffer();
    void fillBuffer(DSP_FLOAT value);

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

class Output : public Connection, public std::enable_shared_from_this<Output> {
    friend class Input;

public:
    Output(unsigned int bufferSize, Type type, Space space = Space::TIME, DSP_FLOAT value = 0.0);
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
