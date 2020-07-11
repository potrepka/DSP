#include "TableOscillator.h"

const unsigned int dsp::TableOscillator::PHASE = 0;
const unsigned int dsp::TableOscillator::POSITION = 1;

dsp::TableOscillator::TableOscillator(Type type) : Generator(type) {
    pushInput(Type::UNIPOLAR);
    pushInput(Type::UNIPOLAR);
}

unsigned int dsp::TableOscillator::getNumTables() const {
    return static_cast<unsigned int>(tables.size());
}

std::shared_ptr<dsp::Buffer> dsp::TableOscillator::getTable(unsigned int index) const {
    return tables[index];
}

void dsp::TableOscillator::setTable(unsigned int index, std::shared_ptr<Buffer> table) {
    lock();
    tables[index] = table;
    unlock();
}

void dsp::TableOscillator::pushTable(std::shared_ptr<Buffer> table) {
    lock();
    tables.push_back(table);
    unlock();
}

void dsp::TableOscillator::insertTable(unsigned int index, std::shared_ptr<Buffer> table) {
    lock();
    tables.insert(tables.begin() + index, table);
    unlock();
}

void dsp::TableOscillator::removeTable(unsigned int index) {
    lock();
    tables.erase(tables.begin() + index);
    unlock();
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::TableOscillator::getPhase() const {
    return getInput(PHASE);
}

std::shared_ptr<dsp::Unit::InputParameter> dsp::TableOscillator::getPosition() const {
    return getInput(POSITION);
}

void dsp::TableOscillator::process() {
    Unit::process();
    if (tables.size() > 0) {
        for (const auto &table : tables) {
            if (table != nullptr) {
                table->lock();
            }
        }
        std::vector<DSP_FLOAT> points;
        points.resize(4);
        for (unsigned int i = 0; i < getNumChannels(); i++) {
            std::vector<DSP_FLOAT> &outputBuffer = getOutputSignal()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &phaseBuffer = getPhase()->getChannel(i)->getBuffer();
            std::vector<DSP_FLOAT> &positionBuffer = getPosition()->getChannel(i)->getBuffer();
            for (unsigned int k = 0; k < getBufferSize(); k++) {
                const DSP_FLOAT positionIndex = clip(positionBuffer[k], 0.0, 1.0) * (tables.size() - 1);
                const long indexBefore = static_cast<long>(positionIndex) - 1;
                long p = indexBefore;
                for (unsigned char j = 0; j < 4; j++) {
                    if (p >= 0 && p < tables.size() && tables[p] != nullptr && tables[p]->getNumChannels() > 0) {
                        points[j] = linear(tables[p]->getChannel(i % tables[p]->getNumChannels()),
                                           wrap(phaseBuffer[k], 0.0, 1.0) * tables[p]->getBufferSize(),
                                           tables[p]->getDefaultValue());
                    } else {
                        points[j] = 0.0;
                    }
                    p++;
                }
                outputBuffer[k] = hermite(points, positionIndex - indexBefore);
            }
        }
        for (const auto &table : tables) {
            if (table != nullptr) {
                table->unlock();
            }
        }
    }
}
