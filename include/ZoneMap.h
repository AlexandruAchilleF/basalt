//
// Created by Flueraru Alexandru Achille on 03/11/2026.
//

#ifndef OOP_ZONEMAP_H
#define OOP_ZONEMAP_H

class ZoneMap {
    double min_;
    double max_;
public:
    ZoneMap(double min, double max);
    bool canBeInside(double lo, double hi) const;
};
#endif //OOP_ZONEMAP_H