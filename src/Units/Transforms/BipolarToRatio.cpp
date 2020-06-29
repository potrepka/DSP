#include "BipolarToRatio.h"

dsp::BipolarToRatio::BipolarToRatio(Connection::Space space)
        : Identity(Connection::Type::BIPOLAR, Connection::Type::RATIO, space) {}
