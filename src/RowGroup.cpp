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

bool RowGroup::canBeInside(std::size_t column, double lo, double hi) const {
    return columnChunks_.at(column).canBeInside(lo, hi);
}

double RowGroup::sumWhere(std::size_t sumColumn, std::size_t filterColumn, double lo, double hi) const {
    std::size_t rowCount = columnChunks_.at(filterColumn).size();
    const ColumnChunk& filterColumnChunk = columnChunks_.at(filterColumn);
    const ColumnChunk& sumColumnChunk = columnChunks_.at(sumColumn);
    double sum = 0.0;
    for (std::size_t row = 0; row < rowCount; ++row) {
        const double value = filterColumnChunk.valueAt(row);
        if (value >= lo && value <= hi) {
            sum += sumColumnChunk.valueAt(row);
        }
    }
    return sum;
}
