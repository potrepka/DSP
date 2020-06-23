#pragma once

#include "Generator.h"

namespace dsp {

class TableOscillator : public Generator {

public:
    TableOscillator();
    std::size_t getNumTables();
    std::vector<DSP_FLOAT> &getTable(std::size_t index);
    void setTable(std::size_t index, std::vector<DSP_FLOAT> &table);
    void pushTable(std::vector<DSP_FLOAT> &table);
    void insertTable(std::size_t index, std::vector<DSP_FLOAT> &table);
    void removeTable(std::size_t index);
    std::shared_ptr<InputParameter> getPhase();
    std::shared_ptr<InputParameter> getPosition();

protected:
    void process() override;

private:
    static const std::size_t PHASE;
    static const std::size_t POSITION;
    std::vector<std::vector<DSP_FLOAT> *> tables;
};

} // namespace dsp
