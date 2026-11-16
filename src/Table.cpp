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
