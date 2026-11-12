//
// Created by Flueraru Alexandru Achille on 13/11/2026.
//

#include "RowGroup.h"
#include <stdexcept>
#include <vector>

namespace {
    const std::vector<ColumnChunk>& checkedChunks(const std::vector<ColumnChunk>& columnChunks) {
        if (columnChunks.empty()) {
            throw std::invalid_argument("RowGroup: needs at least one column chunk");
        }
        const std::size_t rowCount = columnChunks[0].size();
        for (std::size_t i = 1; i < columnChunks.size(); ++i) {
            if (columnChunks[i].size() != rowCount) {
                throw std::invalid_argument("RowGroup: all column chunks must have the same row count");
            }
        }
        return columnChunks;
    }
}

RowGroup::RowGroup(const std::vector<ColumnChunk>& columnChunks)
    : columnChunks_(checkedChunks(columnChunks)) {
}