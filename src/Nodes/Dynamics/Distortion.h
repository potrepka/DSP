#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Distortion : public Transformer {

public:
    Distortion(Space space = Space::TIME);

    std::shared_ptr<Input> getDrive() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> drive;
};

} // namespace dsp
