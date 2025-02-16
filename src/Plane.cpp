#include "geometry/Plane.h"
#include <cmath>

Plane::Plane(float a, float b, float c, const Point& point) : a(a), b(b), c(c), point(point) {}

bool Plane::intersects(const Line& line) const {
    // Simplified intersection logic
    return false;
}

float Plane::distanceTo(const Point& point) const {
    return std::abs(a*point.x + b*point.y + c*point.z) / std::sqrt(a*a + b*b + c*c);
}
