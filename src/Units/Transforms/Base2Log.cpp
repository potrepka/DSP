#include "Base2Log.h"

dsp::Base2Log::Base2Log(Space space)
        : Processor(Type::RATIO, Type::LOGARITHMIC, space) {}

void dsp::Base2Log::process() {
    Unit::process();
#if DSP_USE_VC
    transform([](Vector x) { return Vc::log2(x); });
#else
    transform([](Sample x) { return log2(x); });
#endif
}
