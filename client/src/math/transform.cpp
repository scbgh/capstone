//
// transform.cpp
//

#include "math/transform.h"
#include "common.h"

namespace pg {
namespace math {

//
//
Transform operator*(const Transform& s, const Transform& t)
{
    Transform r = s;
    r *= t;
    return r;
}

//
//
Transform Translate(const Vector& v)
{
    Matrix33 mat {
        1.0, 0.0, v.x,
        0.0, 1.0, v.y,
        0.0, 0.0, 1.0
    };
    Matrix33 matinv {
        1.0, 0.0, -v.x,
        0.0, 1.0, -v.y,
        0.0, 0.0, 1.0 
    };

    return Transform(mat, matinv);
}

//
//
Transform Scale(double x, double y, double z)
{
    Matrix33 mat = Matrix33(
        x, 0.0, 0.0,
        0.0, y, 0.0,
        0.0, 0.0, 1.0
    );
    Matrix33 matinv = Matrix33(
        1.0/x, 0.0, 0.0,
        0.0, 1.0/y, 0.0,
        0.0, 0.0, 1.0
    );

    return Transform(mat, matinv);
}

//
//
Transform Rotate(double t)
{
    double cos_t = cos(DEG_TO_RAD(t));
    double sin_t = sin(DEG_TO_RAD(t));
    Matrix33 mat {
        cos_t, -sin_t, 0.0,
        sin_t, cos_t, 0.0,
        0.0, 0.0, 1.0
    };

    // Rotations are orthonormal, so transpose(A) = inverse(A)
    return Transform(mat, Transpose(mat));
}
//
//
Vector Transform::Apply(const Vector& v) const
{
    Vector w {
        v.x * mat.a[0][0] + v.y * mat.a[0][1],
        v.x * mat.a[1][0] + v.y * mat.a[1][1]
    };

    return w;
}

//
//
Point Transform::Apply(const Point& p) const
{
    Point q {
        p.x * mat.a[0][0] + p.y * mat.a[0][1] + mat.a[0][2],
        p.x * mat.a[1][0] + p.y * mat.a[1][1] + mat.a[1][2]
    };

    return q;
}

}
}
