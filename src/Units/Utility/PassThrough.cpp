#include "PassThrough.h"

dsp::PassThrough::PassThrough(Type type, Space space) : Identity(type, type, space) {}
