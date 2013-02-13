//
// point.cpp
//

#include "math/point.h"
#include "math/vector.h"

namespace pg {
namespace math {

//
//
Point operator+(const Point& p, const Vector& v)
{
    Point u = p;
    u += v;
    return u;
}

//
//
Point operator-(const Point& p, const Vector& v)
{
    Point u = p;
    u -= v;
    return u;
}

//
//
Vector operator-(const Point& p, const Point& q)
{
    return Vector(p.x - q.x, p.y - q.y);
}

//
//
Point operator*(const Point& p, double k)
{
    Point u = p;
    u *= k;
    return u;
}

//
//
Point operator*(double k, const Point& p)
{
    Point u = p;
    u *= k;
    return u;
}

//
//
Point operator/(const Point& p, double k)
{
    Point u = p;
    u /= k;
    return u;
}

}
}
