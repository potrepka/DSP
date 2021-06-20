#pragma once

#include "../Core/Transformer.h"

namespace dsp {

class Spread : public Transformer {

public:
    struct Mode {
        static const int MIN = 0;
        static const int MAX = 1;
        static const int UNIPOLAR = 0;
        static const int BIPOLAR = 1;
    };

    Spread(Type type = Type::RATIO, Space space = Space::TIME);

    std::shared_ptr<Input> getSpread() const;
    std::shared_ptr<Input> getMode() const;

protected:
    void processNoLock() override;

private:
    const std::shared_ptr<Input> spread;
    const std::shared_ptr<Input> mode;
};

} // namespace dsp
