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

namespace pg {
namespace math {

//
// class Transform
// A wrapper for a Matrix which knows its own inverse at
// all times and modifies different types of geometric objects in different
// ways.
class Transform {
public:
    const Matrix33& Matrix() const {
        return mat;
    }

    const Matrix33& InverseMatrix() const {
        return matinv;
    }

    Transform Inverse() const {
        return Transform(matinv, mat);
    }

    Transform& operator*=(const Transform& t) {
        mat = Mult(mat, t.mat);
        matinv = Mult(t.matinv, matinv);
        return *this;
    }

    bool operator==(const Transform& other) const {
        return mat == other.mat && matinv == other.matinv;
    }

    bool operator!=(const Transform& other) const {
        return !(*this == other);
    }

    Vector Apply(const Vector& v) const;
    Point Apply(const Point& p) const;

    Transform(const Matrix33& mat, const Matrix33& matinv)
        : mat(mat),
          matinv(matinv)
    { }

    Transform(const Matrix33& mat)
        : mat(mat)
    {
        matinv = pg::math::Inverse(mat);
    }

    Transform()
        : mat(Matrix33()), matinv(Matrix33())
    { }

private:
    Matrix33 mat, matinv;
};


Transform operator*(const Transform& s, const Transform& t);
Transform Translate(const Vector& v);
Transform Scale(double x, double y, double z);
Transform Rotate(double t);

}
}

#endif
