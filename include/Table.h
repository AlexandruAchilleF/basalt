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
public:
    explicit Table(const std::vector<std::string>& columnNames, const std::vector<RowGroup>& rowGroups);
};

#endif //OOP_TABLE_H