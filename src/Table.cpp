//
// Created by Flueraru Alexandru Achille on 16/11/2026.
//

#include "Table.h"
#include <stdexcept>
#include <vector>
#include <string>
#include <csv.hpp>
#include <date.h>
#include <sstream>

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

    RowGroup makeRowGroup(const std::vector<std::vector<double>>& columns) {
        std::vector<ColumnChunk> chunks;
        for (const auto& column : columns) {
            chunks.push_back(ColumnChunk{column});
        }
        return RowGroup{chunks};
    }

    double toNumber(csv::CSVField field) {
        if (field.is_num()) {
            return field.get<double>();
        }

        date::sys_seconds time;
        std::istringstream iss{field.get<std::string>()};
        iss >> date::parse("%Y-%m-%d %H:%M:%S", time);
        if (iss.fail()) {
            throw std::invalid_argument("Table: field is not a valid date or number: " + field.get<std::string>());
        }
        return static_cast<double>(time.time_since_epoch().count());
    }
}

Table::Table(const std::vector<std::string> &columnNames, const std::vector<RowGroup> &rowGroups)
    : columnNames_(checkedNames(columnNames)), rowGroups_(checkedRowGroups(rowGroups, columnNames_.size())) {
}

Table::Table(const Table &other)
: columnNames_(other.columnNames_), rowGroups_(other.rowGroups_) {
}

void swap(Table &first, Table &second) noexcept {
    using std::swap;
    swap(first.columnNames_, second.columnNames_);
    swap(first.rowGroups_, second.rowGroups_);
}

Table &Table::operator=(Table other) {
    swap(*this, other);
    return *this;
}

Table::~Table() = default;

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

std::size_t Table::countSkipped(const std::string &filterColumn, double lo, double hi) const {
    std::size_t filterIndex = columnIndex(filterColumn);
    std::size_t count = 0;
    for (const auto& rowGroup : rowGroups_) {
        if (!rowGroup.mayContain(filterIndex, lo, hi)) {
            ++count;
        }
    }
    return count;
}

Table Table::fromCsv(const std::string &path) {
    csv::CSVReader reader(path);
    const std::vector<std::string> columnNames = reader.get_col_names();
    std::vector<RowGroup> rowGroups;
    std::vector<std::vector<double>> columns(columnNames.size());
    for (const auto& row : reader) {
        for (std::size_t i = 0; i < columnNames.size(); ++i) {
            columns[i].push_back(toNumber(row[i]));
        }
        if (columns[0].size() == rowsPerGroup) {
            rowGroups.push_back(makeRowGroup(columns));
            for (auto& column : columns) { column.clear(); }
        }
    }
    if (!columns[0].empty()) {
        rowGroups.push_back(makeRowGroup(columns));
    }
    return Table(columnNames, rowGroups);
}

std::ostream& operator<<(std::ostream& os, const Table& table) {
    os << "Table: " << table.rowGroups_.size() << " row groups x " << table.columnNames_.size() << " columns\n";
    for (const auto& columnName : table.columnNames_) {
        os << columnName << " ";
    }
    for (const auto& rowGroup : table.rowGroups_) {
        os << "\n" << rowGroup;
    }
    return os;
}
