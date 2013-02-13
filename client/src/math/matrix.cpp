//
// matrix.cpp
//

#include <algorithm>
#include <cstring>
#include "math/matrix.h"
#include "common.h"

namespace pg {
namespace math {

//
//
Matrix33::Matrix33(double m11, double m12, double m13,
        double m21, double m22, double m23,
        double m31, double m32, double m33)
{
    a[0][0] = m11;
    a[0][1] = m12;
    a[0][2] = m13;
    a[1][0] = m21;
    a[1][1] = m22;
    a[1][2] = m23;
    a[2][0] = m31;
    a[2][1] = m32;
    a[2][2] = m33;
}

//
//
Matrix33::Matrix33()
{
    a[0][0] = 1.0;
    a[0][1] = 0.0;
    a[0][2] = 0.0;
    a[1][0] = 0.0;
    a[1][1] = 1.0;
    a[1][2] = 0.0;
    a[2][0] = 0.0;
    a[2][1] = 0.0;
    a[2][2] = 1.0;
}

//
//
Matrix33 Mult(const Matrix33& l, const Matrix33& r)
{
    Matrix33 product;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            product.a[i][j] = l.a[i][0] * r.a[0][j]
                              + l.a[i][1] * r.a[1][j]
                              + l.a[i][2] * r.a[2][j];
        }
    }

    return product;
}

//
//
Matrix33 Transpose(const Matrix33& m)
{
    Matrix33 transpose;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            transpose.a[i][j] = m.a[j][i];
        }
    }

    return transpose;
}

//
// http://courses.cms.caltech.edu/cs171/c2-1.pdf
Matrix33 Inverse(const Matrix33& m)
{
    Matrix33 minv = m;
    int indxc[3], indxr[3], ipiv[3] = { 0, 0, 0 };
    int irow = 0, icol = 0;

    memcpy(&minv, &m, sizeof(m));

    for (int i = 0; i < 4; ++i) {
        double big = 0.0;
        for (int j = 0; j < 4; ++j) {
            if (ipiv[j] != 1) {
                for (int k = 0; k < 4; ++k) {
                    if (ipiv[k] == 0) {
                        if (fabs(minv.a[j][k]) >= big) {
                            big = fabs(minv.a[j][k]);
                            irow = j;
                            icol = k;
                        }
                    }
                }
            }
        }

        ++ipiv[icol];

        if (irow != icol) {
            for (int j = 0; j < 3; ++j) {
                std::swap(minv.a[irow][j], minv.a[icol][j]);
            }
        }

        indxr[i] = irow;
        indxc[i] = icol;

        if (minv.a[icol][icol] == 0.0) {
            Die("Singular matrix in matrix inversion");
            exit(1);
        }

        double pivinv = 1.0 / minv.a[icol][icol];
        minv.a[icol][icol] = 1.0;
        for (int l = 0; l < 4; ++l) minv.a[icol][l] *= pivinv;
        for (int l = 0; l < 4; ++l) {
            if (l != icol) {
                double dum = minv.a[l][icol];
                minv.a[l][icol] = 0.0;
                for (int ll=0; ll < 4; ++ll) {
                    minv.a[l][ll] -= minv.a[icol][ll] * dum;
                }
            }
        }
    }

    for (int i = 3; i >= 0; --i) {
        if (indxr[i] != indxc[i]) {
            for (int j = 0; j < 4; ++j) {
                std::swap(minv.a[j][indxr[i]], minv.a[j][indxc[i]]);
            }
        }
    }

    return minv;
}

}
}
