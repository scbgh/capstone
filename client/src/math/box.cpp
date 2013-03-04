//
//  box.cpp
//

#include <algorithm>
#include "math/box.h"

namespace pg {
namespace math {

Box AddPoint(const Box& b, Point const& p) {
    Box r = b;
    r.ll.x = std::min(b.ll.x, p.x);
    r.ll.y = std::min(b.ll.y, p.y);
    r.ur.x = std::max(b.ur.x, p.x);
    r.ur.y = std::max(b.ur.y, p.y);
    return r;
}

Box Union(const Box& a, const Box& b) {
    Box r = a;
    r = AddPoint(r, b.ll);
    r = AddPoint(r, b.ur);
    return r;
}

bool Contains(const Box& b, Point const& p) {
    return (p.x >= b.ll.x && p.x <= b.ur.x &&
            p.y >= b.ll.y && p.y <= b.ur.y);
}

bool Overlaps(const Box& b1, const Box& b2) {
    return (b1.ur.x >= b2.ll.x && b1.ll.x <= b2.ur.x &&
            b1.ur.y >= b2.ll.y && b1.ll.y <= b2.ur.y);
}

Box::Box(Point p1, Point p2) {
    ll = p1;
    ur = p2;
}

}
}

