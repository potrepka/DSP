#include "UnipolarToRatio.h"

dsp::UnipolarToRatio::UnipolarToRatio() : Identity(Connection::Type::UNIPOLAR, Connection::Type::RATIO) {}
