//
// polygon.cpp
//

#include "common.h"
#include "polygon.h"

namespace pg {
namespace math {

//
//
double Polygon::Area() const
{
    double area = 0.0;
    int n = points.size();
    for (int i = n-1, j = 0; j < n; i = j++) {
        area += points[i].x * points[j].y - points[i].y * points[j].x;
    }
    return 0.5 * area;
}

//
//
double Polygon::AbsoluteArea() const 
{
    return fabs(Area());
}

//
//
Winding Polygon::WindingDirection() const
{
    return Area() < 0 ? kCW : kCCW;
}

//
//
bool InsideTriangle(Point A, Point B, Point C, Point P)
{
    Point a = C - B;
    Point b = A - C;
    Point c = B - A;
    Point ap = P - A;
    Point bp = P - B;
    Point cp = P - C;

    float axbp = a.x*bp.y - a.y*bp.x;
    float cxap = c.x*ap.y - c.y*ap.x;
    float bxcp = b.x*cp.y - b.y*cp.x;

    return axbp >= 0.0 && bxcp >= 0.0 && cxap >= 0.0;
}

//
//
bool IsEar(const Polygon& poly, int u, int v, int w, int n, int *verts)
{
    int p;
    Point U, V, W, P;

    U = poly[verts[u]];
    V = poly[verts[v]];
    W = poly[verts[w]];

    if ((V.x - U.x) * (W.y - U.y) - (V.y - U.y) * (W.x - U.x) < EPS) {
        return false;
    }

    for (p = 0; p < n; p++) {
        if (p == u || p == v || p == w) {
            continue;
        }

        P = poly[verts[p]];
        if (InsideTriangle(U, V, W, P)) {
            return false;
        }
    }

    return true;
}

//
// http://www.flipcode.com/archives/Efficient_Polygon_Triangulation.shtml
vector<Diagonal> Triangulate(const Polygon& poly)
{
    int n = poly.points.size();
    int *verts = new int[n];
    vector<Diagonal> diags;

    for (int i = 0; i < n; i++) {
        verts[i] = i;
    }

    int remaining = n;
    int m = 0;
    int v = remaining - 1;
    int count = 2*remaining;

    while (remaining > 2) {
        if (count <= 0) {
            Debug("Failed to triangulate polygon");
            return {};
        }

        int u, w;
        u = v;
        if (u >= remaining) u = 0;
        v = u + 1;
        if (v >= remaining) v = 0;
        w = v + 1;
        if (w >= remaining) w = 0;

        if (IsEar(poly, u, v, w, remaining, verts)) {
            int a, b, c;
            a = verts[u];
            b = verts[v];
            c = verts[w];
            diags.push_back({ a, c });

            m++;

            for (int s = v, t = v + 1; t < remaining; s++, t++) {
                verts[s] = verts[t];
                remaining--;
            }

            count = 2 * remaining;
        }
    }

    delete [] verts;
    return diags;
}

//
//
vector<Polygon> Partition(const Polygon& poly, const vector<Diagonals>& diags)
{
    vector<Polygon> polys;
    int *verts = new int[n];

    for (int i = 0; i < n; i++) {
        verts[i] = i;
    }

    for (auto& diag : diags) {

    }

    free verts;
    return polys;
}

}
}
