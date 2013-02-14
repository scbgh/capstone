//
// polygon.h
// Math support for polygons
//

#ifndef _POLYGON_H_
#define _POLYGON_H_

#include <list>
#include <vector>
#include "math/point.h"

namespace pg {
namespace math {

enum Winding {
    kCW,
    kCCW
};

//
// class Polygon
// Represents a sequence of points comprising a polygon
class Polygon {
public:
    std::vector<Point> points;

    Point& operator[](int i) {
        return points[i];
    }

    const Point& operator[](int i) const {
        return points[i];
    }

    double Area() const;
    double AbsoluteArea() const;
    Winding WindingDirection() const;

    void SetWindingDirection(Winding w)
    {
        if (w != WindingDirection()) {
            std::reverse(begin(points), end(points));
        }
    }

    Polygon() { }
    Polygon(const Polygon& other) : points(other.points) { }
    explicit Polygon(const std::vector<Point>& other) : points(other) { }
};

std::list<Polygon> Triangulate(const Polygon& poly);

}
}

#endif
