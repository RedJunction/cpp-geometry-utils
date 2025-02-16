#include "geometry/Point.h"
#include <cmath>

float Point::distanceTo(const Point& other) const {
    return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2) + std::pow(z - other.z, 2));
}

void Point::print() const {
    std::cout << "Point(" << x << ", " << y << ", " << z << ")\n";
}
