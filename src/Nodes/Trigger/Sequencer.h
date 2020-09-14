#pragma once

#include "../Core/Producer.h"

namespace dsp {

class Sequencer : public Producer {

public:
    Sequencer(Type type, Space space = Space::TIME);

    std::vector<std::shared_ptr<Buffer>> &getSequences();

    std::shared_ptr<Input> getSequenceIndex() const;
    std::shared_ptr<Input> getIndex() const;

protected:
    void processNoLock() override;

private:
    std::vector<std::shared_ptr<Buffer>> sequences;
    const std::shared_ptr<Input> sequenceIndex;
    const std::shared_ptr<Input> index;
};

} // namespace dsp