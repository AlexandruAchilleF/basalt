//
// Created by Flueraru Alexandru Achille on 16/11/2026.
//

#ifndef OOP_TABLE_H
#define OOP_TABLE_H

#include "RowGroup.h"
#include <vector>
#include <string>
#include <ostream>
#include <map>

class Table {
    std::vector<std::string> columnNames_;
    std::vector<RowGroup> rowGroups_;
    static constexpr std::size_t rowsPerGroup = 1024;
    std::size_t columnIndex(const std::string& columnName) const;
public:
    explicit Table(const std::vector<std::string>& columnNames, const std::vector<RowGroup>& rowGroups);
    Table(const Table& other);
    friend void swap(Table& first, Table& second) noexcept;
    Table& operator=(Table other);
    ~Table();
    double sumWhere(const std::string& sumColumn, const std::string& filterColumn, double lo, double hi) const;
    std::size_t countSkipped(const std::string& filterColumn, double lo, double hi) const;
    std::map<double, double> sumBy(const std::string& groupColumn, const std::string& sumColumn) const;
    double compressionRatio(const std::string& column) const;
    static Table fromCsv(const std::string& path);
    friend std::ostream& operator<<(std::ostream& os, const Table& table);
};

#endif //OOP_TABLE_H