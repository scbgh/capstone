//
// test.h
// Support functionality for doing unit tests.
//

#ifndef _TESTS_TEST_H_
#define _TESTS_TEST_H_

#include <iostream>
#include <exception>
#include <string>
#include <vector>
#include <map>

namespace test {

// Macro hacks
#define FIXTURE_BEGIN(cls_, name_) \
    public: \
    cls_() \
    { \
        fixture_name = name_;
#define TEST(name_, fn_) AddTest(name_, &fn_)
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
template <typename F>
struct Test {
    std::string name;
    void (F::*func)();
};

//
// struct TestResults
// A structure containing the information about the tests run.
struct TestResults {
    typedef std::map<std::string, TestResult>::iterator iterator;
    std::map<std::string, TestResult> results;
    int total;
    int failure;
};

//
// class Fixture
// A test fixure that contains a number of tests to run.
//
template <typename F>
class Fixture {
public:
    virtual void SetUp() {}
    virtual void TearDown() {}

    const std::string& name() const { return fixture_name; }

    TestResults Run()
    {
        TestResults results;
        results.total = 0;
        results.failure = 0;

        SetUp();

        typename std::vector<Test<F> >::const_iterator it;
        for (it = tests.begin(); it != tests.end(); it++) {
            try {
                results.total++;
                void (F::*func)() = it->func;
                ((F *)this->*func)();
                results.results.insert(std::make_pair(it->name, TestResult("OK", true)));
            } catch (TestResult result) {
                results.failure++;
                results.results.insert(std::make_pair(it->name, result));
            }
        }

        TearDown();

        return results;
    }
protected:
    void AddTest(const std::string& name, void (F::*func)())
    {
        Test<F> t = { name, func };
        tests.push_back(t);
    }
    std::string fixture_name;
    typename std::vector<Test<F> > tests;
};

#define TEST_ASSERT(x) \
    do { \
        if (!(x)) { \
            throw test::TestResult("Assertion failed: " # x, false); \
        } \
    } while (false) \

template <typename F>
TestResults RunFixture()
{
    F fix;
    TestResults results = fix.Run();
    std::cout << "Running test fixture: " << fix.name() << "\n";

    TestResults::iterator it;
    for (it = results.results.begin(); it != results.results.end(); it++) {
        const std::string& name = it->first;
        const TestResult& result = it->second;
        std::cout << "    " << name << "..." << result.message() << "\n";
    }
    std::cout << "Tests run   : " << results.total << "\n";
    std::cout << "Tests failed: " << results.failure << "\n";

    return results;
}

}

#endif
