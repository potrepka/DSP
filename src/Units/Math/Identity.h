#pragma once

#include "Processor.h"

namespace dsp {

class Identity : public Processor {

public:
    Identity(Type inType, Type outType, Space space = Space::TIME);

protected:
    void process() override;
};

class BinaryToLinear : public Identity {

public:
    BinaryToLinear(Space space = Space::TIME);
};

class BinaryToRatio : public Identity {

public:
    BinaryToRatio(Space space = Space::TIME);
};

class IntegerToLinear : public Identity {

public:
    IntegerToLinear(Space space = Space::TIME);
};

class IntegerToRatio : public Identity {

public:
    IntegerToRatio(Space space = Space::TIME);
};

} // namespace dsp
