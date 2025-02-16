#include "geometry/Polygon.h"

void Polygon::addVertex(const Point& point) {
    vertices.push_back(point);
}

float Polygon::area() const {
    // Calculate polygon area (simplified)
    return 0.0f;
}

bool Polygon::containsPoint(const Point& point) const {
    // Check if a point is inside the polygon (simplified)
    return false;
}
