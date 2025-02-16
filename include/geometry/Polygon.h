#ifndef POLYGON_H
#define POLYGON_H

#include "Point.h"
#include <vector>

class Polygon {
public:
    std::vector<Point> vertices;

    void addVertex(const Point& point);
    float area() const;
    bool containsPoint(const Point& point) const;
};

#endif // POLYGON_H
