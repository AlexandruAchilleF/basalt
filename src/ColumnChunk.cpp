//
// Created by Flueraru Alexandru Achille on 04/11/2026.
//

#include "ColumnChunk.h"
#include <stdexcept>
#include <algorithm>
#include <vector>

namespace {
    const std::vector<double>& checkedValues(const std::vector<double> &data) {
        if (data.empty()) {
            throw std::invalid_argument("ColumnChunk: values must not be empty");
        }
         return data;
        }
    }

ColumnChunk::ColumnChunk(const std::vector<double> &data)
: values_(checkedValues(data)), zoneMap_(std::ranges::min(data), std::ranges::max(data)) {
}

bool ColumnChunk::canBeInside(double lo, double hi) const {
    return zoneMap_.canBeInside(lo, hi);
}

double ColumnChunk::valueAt(std::size_t row) const {
    return values_.at(row);
}

std::size_t ColumnChunk::size() const {
    return values_.size();
}