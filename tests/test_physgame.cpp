//
// physgame_test.cpp
// Physgame test harness for performing unit tests.
//

#include <iostream>
#include "test.h"
#include "test_dummy.h"

// MACRO HACK
#define RUN_FIXTURE(cls_) \
	do { \
		test::TestResults results = test::RunFixture<cls_>(); \
		total += results.total; \
		failure += results.failure; \
	} while (false)
/////////////////

int main(int argc, char *argv[])
{
	int total = 0;
	int failure = 0;

	RUN_FIXTURE(TestDummyFixture);

	std::cout << "====================\n";
	std::cout << "\nTOTAL TESTS RUN   : " << total << "\n";
	std::cout << "TOTAL TESTS FAILED: " << failure << "\n\n";
	std::cout << "Done.\n";
}