#ifndef PLANE_H
#define PLANE_H

#include "Point.h"
#include "Line.h"

class Plane {
public:
    Point point;
    float a, b, c; // Normal vector (a, b, c)

    Plane(float a, float b, float c, const Point& point);

    bool intersects(const Line& line) const;
    float distanceTo(const Point& point) const;
};

#endif // PLANE_H
