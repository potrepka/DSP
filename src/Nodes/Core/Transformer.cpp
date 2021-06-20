#include "Transformer.h"

dsp::Transformer::Transformer(Type inType, Type outType, Space space)
        : Node()
        , Consumer(inType, space)
        , Producer(outType, space) {}

dsp::Transformer::Transformer(Type inType, Type outType, Space inSpace, Space outSpace)
        : Node()
        , Consumer(inType, inSpace)
        , Producer(outType, outSpace) {}
