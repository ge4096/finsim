#pragma once

#include "test/unit_test.h"
#include <stdexcept>
#include <string>

class UnitTestTest: public UnitTest {

public:
	bool /* UnitTest:: */ run() override {
		START_TEST(UnitTestTest);
		RUN_TEST_CASE(assertBoolean);
		RUN_TEST_CASE(assertEqual);
		RUN_TEST_CASE(assertComparisons);
		RUN_TEST_CASE(assertThrow);
		END_TEST(UnitTestTest);
	}

private:
	void assertBoolean() {
		ASSERT_TRUE(true);
		ASSERT_FALSE(false);
	}
	
	void assertEqual() {
		ASSERT_EQUAL(0, 0);
		ASSERT_EQUAL(1, 1);
		ASSERT_EQUAL(-1, -1);
		ASSERT_EQUAL(std::string(), std::string());
		ASSERT_EQUAL(std::string("test string"),
		             std::string("test string"));
		
		ASSERT_FLOAT_EQUAL(12.34, 12.34, 0.001);
		ASSERT_FLOAT_EQUAL(12.0, 12.34, 1.0);
	}
	
	void assertComparisons() {
		ASSERT_LESS(0, 1);
		ASSERT_LESS(-1, 0);
		ASSERT_LESS(1.23, 1.234);
		
		ASSERT_GREATER(1, 0);
		ASSERT_GREATER(0, -1);
		ASSERT_GREATER(1.234, 1.23);
	}
	
	void assertThrow() {
		ASSERT_THROW(throw std::runtime_error(""), std::runtime_error);
	}

};
