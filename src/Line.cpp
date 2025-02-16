#include "geometry/Line.h"
#include <cmath>

float Line::length() const {
    return start.distanceTo(end);
}

bool Line::intersects(const Line& other) const {
    // Intersection logic here (simplified for example)
    return false;
}
