//
// Created by Flueraru Alexandru Achille on 04/11/2026.
//

#include "ColumnChunk.h"
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <set>
#include <bit>
#include <climits>

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

bool ColumnChunk::mayContain(double lo, double hi) const {
    return zoneMap_.mayContain(lo, hi);
}

double ColumnChunk::valueAt(std::size_t row) const {
    return values_.at(row);
}

std::size_t ColumnChunk::size() const {
    return values_.size();
}

std::size_t ColumnChunk::dictionaryEncodedBytes() const {
    const std::set<double> uniqueValues(values_.begin(), values_.end());
    const std::size_t bitsPerCode = static_cast<std::size_t>(std::bit_width(uniqueValues.size() - 1));
    const std::size_t codeBytes = (bitsPerCode * values_.size() + CHAR_BIT - 1) / CHAR_BIT;
    const std::size_t dictionaryBytes = uniqueValues.size() * sizeof(double);
    return dictionaryBytes + codeBytes;
}

std::ostream& operator<<(std::ostream& os, const ColumnChunk& columnChunk) {
    os << "ColumnChunk: " << columnChunk.values_.size() << " rows (ZoneMap: " << columnChunk.zoneMap_ << ")";
    return os;
}
