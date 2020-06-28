#include "BipolarToRatio.h"

dsp::BipolarToRatio::BipolarToRatio() : Identity(Connection::Type::BIPOLAR, Connection::Type::RATIO) {}
