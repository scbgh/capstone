#ifndef _TEST_PACK_H_
#define _TEST_PACK_H_

#include "test.h"
#include "file.h"
#include "pack.h"

class TestPackFixture : public test::Fixture
{
    FIXTURE_BEGIN(TestPackFixture);
    TEST(TestPackFixture::EntryCount);
    TEST(TestPackFixture::EntryContents);
    FIXTURE_END();

    pg::Pack *pack;
public:
    virtual void SetUp()
    {
        std::string dir = pg::GetExecutableDirectory();
        std::string path = pg::AppendPath(dir, "test_archive");
        pack = new pg::Pack(path);
    }

    virtual void TearDown()
    {
        delete pack;
    }

    void EntryCount() { TEST_ASSERT_EQ(pack->count(), 3); }
    void EntryContents() { TEST_ASSERT_EQ((*pack)["file2"].ToString(), "Second file\n"); }
};

#endif
