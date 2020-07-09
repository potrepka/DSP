#pragma once

#include "Buffer.h"
#include "Generator.h"

namespace dsp {

class BufferOscillator : public Generator {

public:
    BufferOscillator();
    unsigned int getNumBuffers();
    std::shared_ptr<Buffer> getBuffer(unsigned int index);
    void setBuffer(unsigned int index, std::shared_ptr<Buffer> buffer);
    void pushBuffer(std::shared_ptr<Buffer> buffer);
    void insertBuffer(unsigned int index, std::shared_ptr<Buffer> buffer);
    void removeBuffer(unsigned int index);
    std::shared_ptr<InputParameter> getPhase();
    std::shared_ptr<InputParameter> getPosition();

protected:
    void process() override;

private:
    static const unsigned int PHASE;
    static const unsigned int POSITION;
    std::vector<std::shared_ptr<Buffer>> buffers;
};

} // namespace dsp
