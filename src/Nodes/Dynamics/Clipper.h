#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Clipper : public Transformer {

public:
    enum class Mode { CLIP, WRAP, MIRROR };

    Mode getMode() const;
    void setMode(Mode mode);

    Clipper(Space space = Space::TIME);

protected:
    void processNoLock() override;

private:
    Mode mode;
};

} // namespace dsp
