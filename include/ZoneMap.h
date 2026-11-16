//
// Created by Flueraru Alexandru Achille on 03/11/2026.
//

#ifndef OOP_ZONEMAP_H
#define OOP_ZONEMAP_H

#include <ostream>

class ZoneMap {
    double min_;
    double max_;
public:
    ZoneMap(double min, double max);
    bool mayContain(double lo, double hi) const;
    friend std::ostream& operator<<(std::ostream& os, const ZoneMap& zoneMap);
};
#endif //OOP_ZONEMAP_H