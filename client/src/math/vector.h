//
// vector.h
// Math support for 3D vectors.
//

#ifndef _MATH_VECTOR_H_
#define _MATH_VECTOR_H_

#include <cmath>

namespace pg {
namespace math {

//
// class Vector
// Represents a single vector in a vector space.
class Vector {
public:
    double x, y;

    Vector operator-() const {
        Vector negated = Vector(-x, -y);
        return negated;
    }

    Vector& operator+=(const Vector& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector& operator-=(const Vector& other) {
        *this += -other;
        return *this;
    }

    Vector& operator*=(double other) {
        x *= other;
        y *= other;
        return *this;
    }

    Vector& operator/=(double other) {
        *this *= 1.0/other;
        return *this;
    }

    bool operator==(const Vector& other) {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector& other) {
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

    Vector(double x, double y) : x(x), y(y) { }

    Vector() : x(0.0), y(0.0) { }
};

Vector operator+(const Vector& v, const Vector& w);
Vector operator-(const Vector& v, const Vector& w);
Vector operator*(const Vector& v, double k);
Vector operator*(double k, const Vector& v);
Vector operator/(const Vector& v, double k);
double Dot(const Vector& v, const Vector& w);
double NormSquared(const Vector& v);
double Norm(const Vector& v);
Vector Normalized(const Vector& v);
bool IsZero(const Vector& v);

}
}

#endif
