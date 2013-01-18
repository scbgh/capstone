//
// transform.h
// Math support for linear and affine transformations.
//

#ifndef _MATH_TRANSFORM_H_
#define _MATH_TRANSFORM_H_

#include "math/transform.h"
#include "math/point.h"
#include "math/vector.h"
#include "math/matrix.h"

namespace pg {;

//
// class Transform
// A wrapper for a Matrix which knows its own inverse at
// all times and modifies different types of geometric objects in different
// ways.
class Transform {
public:
  Matrix44 const& Matrix() const {
    return mat;
  }

  Matrix44 const& InverseMatrix() const {
    return matinv;
  }

  Transform Inverse() const {
    return Transform(matinv, mat);
  }

  Transform& operator*=(Transform const& t) {
    mat = Mult(mat, t.mat);
    matinv = Mult(t.matinv, matinv);
    return *this;
  }

  bool operator==(Transform const& other) const {
    return mat == other.mat && matinv == other.matinv;
  }
  
  bool operator!=(Transform const& other) const {
    return !(*this == other);
  }

  Vector Apply(Vector const& v) const;
  Point Apply(Point const& p) const;

  Transform(Matrix44 const& mat, Matrix44 const& matinv)
      : mat(mat),
        matinv(matinv)
  { }

  Transform(Matrix44 const& mat)
      : mat(mat)
  {
    matinv = pg::Inverse(mat);
  }

  Transform()
      : mat(Matrix44()), matinv(Matrix44())
  { }

private:
  Matrix44 mat, matinv;
};


Transform operator*(Transform const& s, Transform const& t);
Transform Translate(Vector const& v);
Transform Scale(double x, double y, double z);
Transform Rotate(Vector const& v, double t);
Transform LookAt(Vector const& eye, Vector const& at, Vector const& up);

}

#endif
