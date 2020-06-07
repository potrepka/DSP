#pragma once

#include <string>
#include <vector>
#include "Constants.h"
#include "Functions.h"
#include "Lockable.h"

namespace dsp {

class Unit;
class Input;
class Output;

class Connection {

public:
    enum class Type {
        BIPOLAR,
        UNIPOLAR,
        SECONDS,
        HERTZ,
        DECIBELS,
        BINARY,
        INTEGER,
        RATIO,
        FFT_REAL,
        FFT_IMAG
    };

    Connection(unsigned int bufferSize, Type type, DSP_FLOAT value);

    unsigned int getBufferSize();
    void setBufferSize(unsigned int bufferSize);
    std::vector<DSP_FLOAT>& getBuffer();
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

class Input : public Connection, public Lockable {
    friend class Output;

public:
    Input(unsigned int bufferSize, Type type, DSP_FLOAT value);
    ~Input();
    std::vector<Output*> getConnections();
    void connect(Output* output);
    void disconnect(Output* output);
    void disconnectAll();
    void copyBuffers();

private:
    std::vector<Output*> connections;
    void addConnection(Output* output);
    void removeConnection(Output* output);

};

class Output : public Connection, public Lockable {
    friend class Input;

public:
    Output(unsigned int bufferSize, Type type, DSP_FLOAT value);
    ~Output();
    std::vector<Input*> getConnections();
    void connect(Input* input);
    void disconnect(Input* input);
    void disconnectAll();

private:
    std::vector<Input*> connections;
    void addConnection(Input* input);
    void removeConnection(Input* input);

};

void operator>>(DSP_FLOAT value, Input& input);
void operator>>(Output& output, Input& input);
void operator!=(Output& output, Input& input);

}
