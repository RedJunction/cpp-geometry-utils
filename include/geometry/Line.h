#ifndef LINE_H
#define LINE_H

#include "Point.h"

class Line {
public:
    Point start, end;

    Line(const Point& start, const Point& end) : start(start), end(end) {}

    float length() const;
    bool intersects(const Line& other) const;
};

#endif // LINE_H
