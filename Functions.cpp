#include "Functions.h"

DSP_FLOAT dsp::toAmplitude(DSP_FLOAT decibels) {
    return pow(10.0, 0.05 * decibels);
}

DSP_FLOAT dsp::toDecibels(DSP_FLOAT amplitude) {
    return 20.0 * log10(amplitude);
}

DSP_FLOAT dsp::toHertz(DSP_FLOAT seconds) {
    return 1.0 / seconds;
}

DSP_FLOAT dsp::toSeconds(DSP_FLOAT hertz) {
    return 1.0 / hertz;
}

DSP_FLOAT dsp::linear(std::vector<DSP_FLOAT> &table, const DSP_FLOAT index) {
    int indexFloor = static_cast<int>(index);
    DSP_FLOAT mu = index - indexFloor;
    DSP_FLOAT x1 = table[indexFloor % table.size()];
    DSP_FLOAT x2 = table[(indexFloor + 1) % table.size()];
    return x1 + mu * (x2 - x1);
}

DSP_FLOAT dsp::hermite(std::vector<DSP_FLOAT> &table, const DSP_FLOAT index) {
    int indexFloor = static_cast<int>(index);
    DSP_FLOAT mu = index - indexFloor;
    DSP_FLOAT x0 = table[(indexFloor + table.size() - 1) % table.size()];
    DSP_FLOAT x1 = table[indexFloor % table.size()];
    DSP_FLOAT x2 = table[(indexFloor + 1) % table.size()];
    DSP_FLOAT x3 = table[(indexFloor + 2) % table.size()];
    DSP_FLOAT a = 0.5 * (3.0 * (x1 - x2) - x0 + x3);
    DSP_FLOAT b = x2 + x2 + x0 - 0.5 * (5.0 * x1 + x3);
    DSP_FLOAT c = 0.5 * (x2 - x0);
    return ((a * mu + b) * mu + c) * mu + x1;
}
