#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Clipper : public Transformer {

public:
    enum class Mode { CLIP, WRAP, MIRROR };

    Clipper(Space space = Space::TIME);

    Mode getMode() const;
    void setMode(Mode mode);

protected:
    void processNoLock() override;

private:
    Mode mode;
};

} // namespace dsp
