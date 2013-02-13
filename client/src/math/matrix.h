//
// matrix.h
// Math support for 4D matrices
//

#ifndef _MATH_MATRIX_H_
#define _MATH_MATRIX_H_

namespace pg {
namespace math {

//
// class Matrix33
// 3x3 matrix for 2D math.
class Matrix33 {
public:
    double a[3][3];

    bool operator==(const Matrix33& other) const {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (a[i][j] != other.a[i][j]) {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator!=(const Matrix33& other) const {
        return !(*this == other);
    }

    Matrix33(double m11, double m12, double m13,
             double m21, double m22, double m23,
             double m31, double m32, double m33);

    Matrix33();
};

Matrix33 Mult(const Matrix33& l, const Matrix33& r);
Matrix33 Transpose(const Matrix33& m);
Matrix33 Inverse(const Matrix33& m);

}
}

#endif
