#pragma once

#include "Buffer.h"
#include "Generator.h"

namespace dsp {

class TableOscillator : public Generator {

public:
    TableOscillator(Type type);

    unsigned int getNumTables() const;
    std::shared_ptr<Buffer> getTable(unsigned int index) const;
    std::vector<std::shared_ptr<Buffer>> getTables(unsigned int begin, unsigned int end) const;
    void pushTable(std::shared_ptr<Buffer> table);
    void pushTables(std::vector<std::shared_ptr<Buffer>> tables);
    void removeTable(std::shared_ptr<Buffer> table);
    void removeTables(std::vector<std::shared_ptr<Buffer>> tables);
    std::shared_ptr<InputParameter> getPhase() const;
    std::shared_ptr<InputParameter> getPosition() const;

protected:
    void process() override;

private:
    const std::shared_ptr<InputParameter> phase;
    const std::shared_ptr<InputParameter> position;

    std::vector<std::shared_ptr<Buffer>> buffers;
};

} // namespace dsp
