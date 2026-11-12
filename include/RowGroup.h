//
// Created by Flueraru Alexandru Achille on 13/11/2026.
//

#ifndef OOP_ROWGROUP_H
#define OOP_ROWGROUP_H

#include "ColumnChunk.h"
#include <vector>

class RowGroup {
    std::vector<ColumnChunk> columnChunks_;
public:
    explicit RowGroup(const std::vector<ColumnChunk>& columnChunks);
};


#endif //OOP_ROWGROUP_H