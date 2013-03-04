#ifndef _BOX_H_
#define _BOX_H_

#include <algorithm>
#include "math/point.h"
#include "common.h"

namespace pg {
namespace math {

class Box {
public:
    Point ll;
    Point ur;

    double Width() const { return ur.x - ll.x; }
    double Height() const { return ur.y - ll.y; }

    Box(Point p) : ll(p), ur(p) { }
    Box(Point p1, Point p2);
    Box() :
        ll(Point(INFINITY, INFINITY)),
        ur(Point(-INFINITY, -INFINITY))
    { }
    Box(const Box& box) : ll(box.ll), ur(box.ur) { }
};

Box AddPoint(const Box& b, Point const& p);
Box Union(const Box& a, const Box& b);
bool Contains(const Box& b, Point const& p);
bool Overlaps(const Box& b1, const Box& b2);

}
}

#endif

