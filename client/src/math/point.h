//
// point.h
// Math support for 2D points
//

#ifndef _MATH_POINT_H_
#define _MATH_POINT_H_

#include "math/vector.h"

namespace pg {
namespace math {

//
// class Point
// Represents a single point in affine space. Points can be subtracted
// but not added together and are affected by translation transformations.
class Point {
public:
    double x, y;

    Point operator-() const {
        Point negated = Point(-x, -y);
        return negated;
    }

    Point& operator+=(const Vector& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point& operator-=(const Vector& other) {
        *this += -other;
        return *this;
    }

    Point& operator*=(double other) {
        x *= other;
        y *= other;
        return *this;
    }

    Point& operator/=(double other) {
        *this *= 1.0/other;
        return *this;
    }

    bool operator==(const Point& other) {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) {
        return !(*this == other);
    }

    double& operator[](int i) {
        if (i & 1) return y;
        return x;
    }

    const double& operator[](int i) const {
        if (i & 1) return y;
        return x;
    }

    operator Vector() {
        return Vector(x, y);
    }

    Point(double x, double y) : x(x), y(y) { }

    Point() : x(0.0), y(0.0) { }

    Point(const Vector& vector) : x(vector.x), y(vector.y) { }
};

Point operator+(const Point& p, const Vector& v);
Point operator-(const Point& p, const Vector& v);
Vector operator-(const Point& p, const Point& q);
Point operator*(const Point& p, double k);
Point operator*(double k, const Point& p);
Point operator/(const Point& p, double k);

}
}

#endif
