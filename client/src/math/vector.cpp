//
// vector.cpp
//

#include "common.h"
#include "math/vector.h"

#include <math.h>

namespace pg {
namespace math {

//
//
Vector operator+(const Vector& v, const Vector& w)
{
    Vector u = v;
    u += w;
    return u;
}

//
//
Vector operator-(const Vector& v, const Vector& w)
{
    Vector u = v;
    u -= w;
    return u;
}

//
//
Vector operator*(const Vector& v, double k)
{
    Vector u = v;
    u *= k;
    return u;
}

//
//
Vector operator*(double k, const Vector& v) 
{
    Vector u = v;
    u *= k;
    return u;
}

//
//
Vector operator/(const Vector& v, double k)
{
    Vector u = v;
    u /= k;
    return u;
}

//
//
double Dot(const Vector& v, const Vector& w)
{
    return v.x * w.x + v.y * w.y;
}

//
//
double NormSquared(const Vector& v)
{
    return Dot(v, v);
}

//
//
double Norm(const Vector& v)
{
    return sqrt(NormSquared(v));
}

//
//
Vector Normalized(const Vector& v)
{
    return v / Norm(v);
}

//
//
bool IsZero(const Vector& v)
{
    return v.x == 0 && v.y == 0;
}

}
}
