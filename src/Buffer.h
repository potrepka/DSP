#pragma once

#include "Constants.h"
#include "Functions.h"
#include "Lockable.h"

namespace dsp {

class Buffer : public Lockable {

public:
    Buffer(Type type,
           Space space = Space::TIME,
           Sample defaultValue = 0.0,
           size_t numChannels = 0,
           size_t numSamples = 0);

    Type getType() const;
    void setType(Type type);

    Space getSpace() const;
    void setSpace(Space space);

    Sample getDefaultValue() const;
    void setDefaultValue(Sample defaultValue);

    size_t getNumChannels() const;
    void setNumChannels(size_t numChannels);

    size_t getNumSamples() const;
    void setNumSamples(size_t numSamples);

    void setSize(size_t numChannels, size_t numSamples);

    Array getChannelValues() const;
    void setChannelValues(Array value);

    Sample getChannelValue(size_t channel) const;
    void setSingleChannelValue(size_t channel, Sample value);
    void setAllChannelValues(Sample value);

    Array getPeak();
    Array getRMS();

    Wrapper &getWrapper();

protected:
    Type type;
    Space space;
    Sample defaultValue;
    Data data;
    Wrapper wrapper;
    Array channelValues;

    void fillChannels();
};

class Input;
class Output;

class Input : public Buffer, public std::enable_shared_from_this<Input> {
    friend class Output;

public:
    enum class Mode { SUM, MINIMUM, MAXIMUM };

    Input(Type type,
          Space space = Space::TIME,
          Sample defaultValue = 0.0,
          size_t numChannels = 0,
          size_t numSamples = 0);
    ~Input();

    Mode getMode() const;
    void setMode(Mode mode);

    std::vector<std::shared_ptr<Output>> getConnections() const;
    void connect(std::shared_ptr<Output> output);
    void disconnect(std::shared_ptr<Output> output);
    void disconnectAll();
    void processNoLock();

private:
    Mode mode;
    std::vector<std::shared_ptr<Output>> connections;

    void addConnection(std::shared_ptr<Output> output);
    void removeConnection(std::shared_ptr<Output> output);
};

class Output : public Buffer, public std::enable_shared_from_this<Output> {
    friend class Input;

public:
    Output(Type type,
           Space space = Space::TIME,
           Sample defaultValue = 0.0,
           size_t numChannels = 0,
           size_t numSamples = 0);
    ~Output();

    std::vector<std::shared_ptr<Input>> getConnections() const;
    void connect(std::shared_ptr<Input> input);
    void disconnect(std::shared_ptr<Input> input);
    void disconnectAll();
    void processNoLock();

private:
    std::vector<std::shared_ptr<Input>> connections;

    void addConnection(std::shared_ptr<Input> input);
    void removeConnection(std::shared_ptr<Input> input);
};

void operator>>(Sample value, std::shared_ptr<Input> input);
void operator>>(Sample value, std::shared_ptr<Output> output);

void operator>>(Array values, std::shared_ptr<Input> input);
void operator>>(Array values, std::shared_ptr<Output> output);

void operator>>(std::shared_ptr<Output> output, std::shared_ptr<Input> input);
void operator!=(std::shared_ptr<Output> output, std::shared_ptr<Input> input);

} // namespace dsp
