#include <algorithm>
#include "math/color.h"

namespace pg {
namespace math {

//
//
Color operator+(const Color& a, const Color& b) {
    Color c = a;
    c += b;
    return c;
}

//
//
Color operator-(const Color& a, const Color& b) {
    Color c = a;

    c -= b;
    return c;
}

//
//
Color operator*(const Color& a, const Color& b) {
    Color c = a;
    c *= b;
    return c;
}

//
//
Color operator*(const Color& a, double k) {
    Color c = a;
    c *= k;
    return c;
}

//
//
Color operator*(double k, const Color& a) {
    Color c = a;
    c *= k;
    return c;
}

//
//
Color operator/(const Color& a, const Color& b) {
    Color c = a;
    c /= b;
    return c;
}

//
//
Color operator/(const Color& a, double k) {
    Color c = a;
    c /= k;
    return c;
}

//
//
Color Clamp(const Color& a) {
    Color b;
    b.r = std::min(std::max(a.r, 0.0), 1.0);
    b.g = std::min(std::max(a.g, 0.0), 1.0);
    b.b = std::min(std::max(a.b, 0.0), 1.0);
    b.a = std::min(std::max(a.a, 0.0), 1.0);

    return b;
}

}
}
