//
// Created by Flueraru Alexandru Achille on 03/11/2026.
//

#include "ZoneMap.h"

#include <stdexcept>

ZoneMap::ZoneMap(double min, double max)
    : min_(min), max_(max)
{
    if (min_ > max_)
        throw std::invalid_argument("ZoneMap: min must be <= max");
}

bool ZoneMap::canBeInside(double lo, double hi) const {
    if (lo > hi)
        return false;
    if (hi < min_ || lo > max_)
        return false;
    return true;
}
