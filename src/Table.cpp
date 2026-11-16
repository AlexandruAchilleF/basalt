//
// Created by Flueraru Alexandru Achille on 16/11/2026.
//

#include "Table.h"
#include <stdexcept>
#include <vector>
#include <string>

namespace {
    const std::vector<std::string>& checkedNames(const std::vector<std::string> &names) {
        if (names.empty()) {
            throw std::invalid_argument("Table: needs at least one column name");
        }
        return names;
    }
    const std::vector<RowGroup>& checkedRowGroups(const std::vector<RowGroup>& rowGroups, const std::size_t columnCount) {
        if (rowGroups.empty()) {
            throw std::invalid_argument("Table: needs at least one row group");
        }

        for (const auto& rowGroup : rowGroups) {
            if (rowGroup.columnCount() != columnCount) {
                throw std::invalid_argument("Table: all row groups must have the same number of columns");
            }
        }
        return rowGroups;
    }
}

Table::Table(const std::vector<std::string> &columnNames, const std::vector<RowGroup> &rowGroups)
    : columnNames_(checkedNames(columnNames)), rowGroups_(checkedRowGroups(rowGroups, columnNames_.size())) {
}

std::size_t Table::columnIndex(const std::string &columnName) const {
    for (std::size_t i = 0; i < columnNames_.size(); ++i) {
        if (columnNames_[i] == columnName) {
            return i;
        }
    }
    throw std::invalid_argument("Table: unknown column " + columnName);
}

double Table::sumWhere(const std::string &sumColumn, const std::string &filterColumn, double lo, double hi) const {
    std::size_t sumIndex = columnIndex(sumColumn);
    std::size_t filterIndex = columnIndex(filterColumn);
    double total = 0.0;
    for (const auto& rowGroup : rowGroups_) {
        if (rowGroup.mayContain(filterIndex, lo, hi)) {
            total += rowGroup.sumWhere(sumIndex, filterIndex, lo, hi);
        }
    }
    return total;
}
