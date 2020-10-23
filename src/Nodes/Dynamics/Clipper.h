#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Clipper : public Transformer {

public:
    enum class Mode { CLIP, WRAP, MIRROR };

    Clipper(Type type = Type::RATIO, Space space = Space::TIME);

    Mode getMode() const;
    void setMode(Mode mode);

    std::shared_ptr<Input> getMinimum() const;
    std::shared_ptr<Input> getMaximum() const;

protected:
    void processNoLock() override;

private:
    Mode mode;
    std::shared_ptr<Input> minimum;
    std::shared_ptr<Input> maximum;
};

} // namespace dsp
