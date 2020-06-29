#include "UnipolarToRatio.h"

dsp::UnipolarToRatio::UnipolarToRatio(Connection::Space space)
        : Identity(Connection::Type::UNIPOLAR, Connection::Type::RATIO, space) {}
