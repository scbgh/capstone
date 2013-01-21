#ifndef _TEST_DUMMY_H_
#define _TEST_DUMMY_H_

#include "test.h"

class TestDummyFixture : public test::Fixture
{
    FIXTURE_BEGIN(TestDummyFixture);
    TEST(TestDummyFixture::Dummy1);
    TEST(TestDummyFixture::Dummy2);
    FIXTURE_END();

    int val;
public:
    virtual void SetUp() {
        val = 123;
    }

    void Dummy1() { TEST_ASSERT(val == 123); }
    void Dummy2() { TEST_ASSERT(val < 500); }
};

#endif
