#pragma once

#include "../Core/Producer.h"

namespace dsp {

class MultiplyHertzSeconds : public Producer {

public:
    MultiplyHertzSeconds(Space space = Space::TIME);

    std::shared_ptr<Input> getFrequency() const;
    std::shared_ptr<Input> getTime() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> frequency;
    const std::shared_ptr<Input> time;
};

} // namespace dsp
