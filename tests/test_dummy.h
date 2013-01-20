#ifndef _TEST_DUMMY_H_
#define _TEST_DUMMY_H_

#include "test.h"

using namespace test;

class TestDummyFixture : public Fixture<TestDummyFixture>
{
    FIXTURE_BEGIN(TestDummyFixture, "Dummy test");
    TEST("First dummy test", TestDummyFixture::Dummy1);
    TEST("Second dummy test", TestDummyFixture::Dummy2);
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
