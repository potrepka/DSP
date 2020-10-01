#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Spread : public Transformer {

public:
    enum class Mode { UNIPOLAR, BIPOLAR };

    Spread(Type type, Space space = Space::TIME);

    Mode getMode() const;
    void setMode(Mode mode);

    std::shared_ptr<Input> getSpread() const;

protected:
    void processNoLock() override;

private:
    Mode mode;
    const std::shared_ptr<Input> spread;
};

} // namespace dsp
