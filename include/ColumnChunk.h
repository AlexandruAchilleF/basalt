//
// Created by Flueraru Alexandru Achille on 04/11/2026.
//

#ifndef OOP_COLUMNCHUNK_H
#define OOP_COLUMNCHUNK_H
#include <vector>

#include "ZoneMap.h"

class ColumnChunk {
    std::vector<double> values_;
    ZoneMap zoneMap_;
public:
    explicit ColumnChunk(const std::vector<double> &data);
    bool canBeInside(double lo, double hi) const;
    std::size_t size() const;
    double valueAt(std::size_t row) const;
};

#endif //OOP_COLUMNCHUNK_H