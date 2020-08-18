#pragma once

#include "Buffer.h"
#include "Generator.h"

namespace dsp {

class Sequencer : public Generator, public BufferCollection {

public:
    Sequencer(Type type, Space space = Space::TIME);

    unsigned int getNumSequences() const;
    std::shared_ptr<Buffer> getSequence(unsigned int index) const;
    std::vector<std::shared_ptr<Buffer>> getSequences(unsigned int begin, unsigned int end) const;
    void pushSequence(std::shared_ptr<Buffer> sequence);
    void pushSequences(std::vector<std::shared_ptr<Buffer>> sequences);
    void replaceSequence(std::shared_ptr<Buffer> sequence, std::shared_ptr<Buffer> replacement);
    void removeSequence(std::shared_ptr<Buffer> sequence);
    void removeSequences(std::vector<std::shared_ptr<Buffer>> sequences);

    std::shared_ptr<InputParameter> getSequenceIndex() const;
    std::shared_ptr<InputParameter> getIndex() const;

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> sequenceIndex;
    const std::shared_ptr<InputParameter> index;
};

} // namespace dsp
