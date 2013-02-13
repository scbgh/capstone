#ifndef _TEST_POLYGON_H_
#define _TEST_POLYGON_H_

#include "test.h"
#include "math/math.h"

class TestPolygonFixture : public test::Fixture
{
    FIXTURE_BEGIN(TestPolygonFixture);
    TEST(TestPolygonFixture::Winding);
    TEST(TestPolygonFixture::Area);
    FIXTURE_END();

    pg::math::Polygon poly;
public:
    virtual void SetUp()
    {
        poly.points.push_back({ 1.0, 0.0 });
        poly.points.push_back({ 0.0, -1.0 });
        poly.points.push_back({ -1.0, 0.0 });
        poly.points.push_back({ 0.0, 1.0 });
    }

    virtual void TearDown()
    {
    }

    void Winding() { TEST_ASSERT_EQ(poly.WindingDirection(), pg::math::kCW); }
    void Area() { TEST_ASSERT_EQ(poly.Area(), -2.0); }
};

#endif
