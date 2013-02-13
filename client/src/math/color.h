//
// color.h
// Math support for RGBA colours.
//

#ifndef _MATH_COLOR_H_
#define _MATH_COLOR_H_

namespace pg {
namespace math {

//
// class Color
// Represents a single RGBA colour.
class Color {
public:
    double r, g, b, a;

    Color& operator+=(const Color& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        a += other.a;

        return *this;
    }

    Color& operator-=(const Color& other) {
        r -= other.r;
        g -= other.g;
        b -= other.b;
        a -= other.a;

        return *this;
    }

    Color& operator*=(const Color& other) {
        r *= other.r;
        g *= other.g;
        b *= other.b;
        a *= other.a;

        return *this;
    }

    Color& operator*=(double k) {
        r *= k;
        g *= k;
        b *= k;
        a *= k;

        return *this;
    }

    Color& operator/=(const Color& other) {
        r /= other.r;
        g /= other.g;
        b /= other.b;
        a /= other.a;

        return *this;
    }

    Color& operator/=(double k) {
        r /= k;
        g /= k;
        b /= k;
        a /= k;

        return *this;
    }

    bool operator==(const Color& other) {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    bool operator!=(const Color& other) {
        return !(*this == other);
    }

    explicit Color(double r = 0.0, double g = 0.0, double b = 0.0)
        : r(r), g(g), b(b)
    { }
};

Color operator+(const Color& a, const Color& b);
Color operator-(const Color& a, const Color& b);
Color operator*(const Color& a, const Color& b);
Color operator*(const Color& a, double k);
Color operator*(double k, const Color& a);
Color operator/(const Color& a, const Color& b);
Color operator/(const Color& a, double k);

Color Clamp(const Color& a);
#endif

}
}
