//
// Created by Flueraru Alexandru Achille on 13/11/2026.
//

#ifndef OOP_ROWGROUP_H
#define OOP_ROWGROUP_H

#include "ColumnChunk.h"
#include <vector>
#include <ostream>

class RowGroup {
    std::vector<ColumnChunk> columnChunks_;
public:
    explicit RowGroup(const std::vector<ColumnChunk>& columnChunks);
    bool mayContain(std::size_t column, double lo, double hi) const;
    double sumWhere(std::size_t sumColumn, std::size_t filterColumn, double lo, double hi) const;
    std::size_t columnCount() const;
    friend std::ostream& operator<<(std::ostream& os, const RowGroup& rowGroup);
};


#endif //OOP_ROWGROUP_H