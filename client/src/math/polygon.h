//
// polygon.h
// Math support for polygons
//

#ifndef _POLYGON_H_
#define _POLYGON_H_

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
    Polygon(const std::vector<Point>& other) : points(other) { }
};

//
// struct Diagonal
// Represents a diagonal between two indexed points in a polygon
struct Diagonal {
    int p1;
    int p2;
};

std::vector<Diagonal> Triangulate(const Polygon& poly);
std::vector<Diagonal> ConvexDecompose(const Polygon& poly);
std::vector<Polygon> Partition(const Polygon& poly, const std::vector<Diagonal>& diags);

}
}

#endif
