#include "TableOscillator.h"

const std::size_t dsp::TableOscillator::PHASE = 0;
const std::size_t dsp::TableOscillator::POSITION = 1;

dsp::TableOscillator::TableOscillator() : Generator(Connection::Type::BIPOLAR) {
    pushInput(Connection::Type::UNIPOLAR);
    pushInput(Connection::Type::UNIPOLAR);
}

std::size_t dsp::TableOscillator::getNumTables() {
    return tables.size();
}

std::vector<DSP_FLOAT> dsp::TableOscillator::getTable(std::size_t index) {
    return tables[index];
}

void dsp::TableOscillator::setTable(std::size_t index, const std::vector<DSP_FLOAT> &table) {
    lock();
    tables[index] = table;
    unlock();
}

void dsp::TableOscillator::pushTable(const std::vector<DSP_FLOAT> &table) {
    lock();
    tables.push_back(table);
    unlock();
}

void dsp::TableOscillator::insertTable(std::size_t index, const std::vector<DSP_FLOAT> &table) {
    lock();
    tables.insert(tables.begin() + index, table);
    unlock();
}

void dsp::TableOscillator::removeTable(std::size_t index) {
    lock();
    tables.erase(tables.begin() + index);
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::TableOscillator::getPhase() {
    return getInput(PHASE);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::TableOscillator::getPosition() {
    return getInput(POSITION);
}

void dsp::TableOscillator::process() {
    Unit::process();
    if (tables.size() > 0) {
        std::vector<DSP_FLOAT> points;
        points.resize(4);
        for (std::size_t i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &positionBuffer = getPosition()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                const DSP_FLOAT positionIndex = positionBuffer[k] * (tables.size() - 1);
                const std::size_t indexBefore = static_cast<std::size_t>(positionIndex) - 1;
                std::size_t index = indexBefore;
                for (unsigned char j = 0; j < 4; j++) {
                    if (index >= 0 && index < tables.size() && tables[index].size() > 0) {
                        points[j] = linear(tables[index], phaseBuffer[k] * tables[index].size());
                    } else {
                        points[j] = 0.0;
                    }
                    index++;
                }
                outputBuffer[k] = hermite(points, positionIndex - indexBefore);
            }
        }
    }
}
