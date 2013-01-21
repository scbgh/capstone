//
// test.h
// Support functionality for doing unit tests.
//

#ifndef _TESTS_TEST_H_
#define _TESTS_TEST_H_

#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace test {

// Macro hacks
#define FIXTURE_BEGIN(classname) \
    public: \
    classname() : Fixture(#classname) \
    {
#define TEST(fn) AddTest(#fn, std::bind(&fn, this))
#define FIXTURE_END() \
    } \
    private: \
    typedef int DUMMY // dummy typedef to allow semicolon



//
// class TestResult
// A container for a message about the result of a test.
//
class TestResult {
public:
    TestResult(const std::string& message, bool success) :
        message_(message),
        success_(success)
    { }

    const std::string& message() const { return message_; }
    bool success() const { return success_; }
private:
    std::string message_;
    bool success_;
};

//
// struct Test
// A single test belonging to a fixture.
//
struct Test {
    std::string name;
    std::function<void()> func;
};

//
// struct TestResults
// A structure containing the information about the tests run.
//
struct TestResults {
    std::map<std::string, TestResult> results;
    int total;
    int failure;

    TestResults() :
        total(0),
        failure(0)
    {
    }
};

//
// class Fixture
// A test fixure that contains a number of tests to run.
//
class Fixture {
public:
    explicit Fixture(const std::string& name) :
        fixture_name(name)
    {
    }

    virtual void SetUp() {}
    virtual void TearDown() {}

    const std::string& name() const { return fixture_name; }

    TestResults Run()
    {
        TestResults results;

        SetUp();

        for (auto& test : tests) {
            try {
                results.total++;
                test.func();
                results.results.insert(std::make_pair(test.name, TestResult("OK", true)));
            } catch (TestResult result) {
                results.failure++;
                results.results.insert(std::make_pair(test.name, result));
            }
        }

        TearDown();

        return results;
    }
protected:
    void AddTest(const std::string& name, std::function<void()> func)
    {
        tests.push_back({ name, func });
    }

    const std::string fixture_name;
    std::vector<Test> tests;
};

#define TEST_ASSERT(x) \
    do { \
        if (!(x)) { \
            throw test::TestResult("Assertion failed: " # x, false); \
        } \
    } while (false)

template <typename F>
TestResults RunFixture()
{
    F fix;
    TestResults results = fix.Run();
    std::cout << "Running test fixture: " << fix.name() << "\n";

    for (auto& result_entry : results.results) {
        std::cout << "    " << result_entry.first << "..." << result_entry.second.message() << "\n";
    }
    std::cout << "Tests run   : " << results.total << "\n";
    std::cout << "Tests failed: " << results.failure << "\n";

    return results;
}

}

#endif
