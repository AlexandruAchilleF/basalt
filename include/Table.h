//
// Created by Flueraru Alexandru Achille on 16/11/2026.
//

#ifndef OOP_TABLE_H
#define OOP_TABLE_H

#include "RowGroup.h"
#include <vector>
#include <string>

class Table {
    std::vector<std::string> columnNames_;
    std::vector<RowGroup> rowGroups_;
    static constexpr std::size_t rowsPerGroup = 1024;
    std::size_t columnIndex(const std::string& columnName) const;
public:
    explicit Table(const std::vector<std::string>& columnNames, const std::vector<RowGroup>& rowGroups);
    double sumWhere(const std::string& sumColumn, const std::string& filterColumn, double lo, double hi) const;
    std::size_t countSkipped(const std::string& filterColumn, double lo, double hi) const;
    static Table fromCsv(const std::string& path);
};

#endif //OOP_TABLE_H