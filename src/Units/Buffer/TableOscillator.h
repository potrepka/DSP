#pragma once

#include "Generator.h"

namespace dsp {

class TableOscillator : public Generator {

public:
    TableOscillator();
    unsigned int getNumTables();
    std::vector<DSP_FLOAT> &getTable(unsigned int index);
    void setTable(unsigned int index, std::vector<DSP_FLOAT> &table);
    void pushTable(std::vector<DSP_FLOAT> &table);
    void insertTable(unsigned int index, std::vector<DSP_FLOAT> &table);
    void removeTable(unsigned int index);
    std::shared_ptr<InputParameter> getPhase();
    std::shared_ptr<InputParameter> getPosition();

protected:
    void process() override;

private:
    static const unsigned int PHASE;
    static const unsigned int POSITION;
    std::vector<std::vector<DSP_FLOAT> *> tables;
};

} // namespace dsp
