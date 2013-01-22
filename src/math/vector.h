//
// vector.h
// Math support for 3D vectors.
//

#ifndef _MATH_VECTOR_H_
#define _MATH_VECTOR_H_

#include <cmath>

namespace pg { namespace math {

//
// class Vector
// Represents a single vector in a vector space.
//
class Vector {
public:
  double x, y, z;

  Vector operator-() const {
    Vector negated = Vector(-x, -y, -z);
    return negated;
  }

  Vector& operator+=(Vector const& other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
  }

  Vector& operator-=(Vector const& other) {
    *this += -other;
    return *this;
  }

  Vector& operator*=(double other) {
    x *= other;
    y *= other;
    z *= other;
    return *this;
 }

  Vector& operator/=(double other) {
    *this *= 1.0/other;
    return *this;
  }

  bool operator==(Vector const& other) {
    return x == other.x && y == other.y && z == other.z;
  }
  
  bool operator!=(Vector const& other) {
    return !(*this == other);
  }

  double& operator[](int i) {
    switch (i%3) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      return z;
    }
  }

  double const& operator[](int i) const {
    switch (i%3) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      return z;
    }
  }

  Vector(double x, double y) : x(x), y(y), z(0.0) { }

  Vector(double x, double y, double z) : x(x), y(y), z(z) { }

  Vector() : x(0.0), y(0.0), z(0.0) { }
};

Vector operator+(Vector const& v, Vector const& w);
Vector operator-(Vector const& v, Vector const& w);
Vector operator*(Vector const& v, double k);
Vector operator*(double k, Vector const& v);
Vector operator/(Vector const& v, double k);
double Dot(Vector const& v, Vector const& w);
Vector Cross(Vector const& v, Vector const& w);
double NormSquared(Vector const& v);
double Norm(Vector const& v);
Vector Normalized(Vector const& v);
bool IsZero(Vector const& v);

} }

#endif
