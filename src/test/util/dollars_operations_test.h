#pragma once

#include "test/unit_test.h"
#include "util/dollars.h"
#include <sstream>
#include <stdexcept>

class DollarsOperationsTest: public UnitTest {

public:
	bool /* UnitTest:: */ run() override {
		START_TEST(DollarsOperationsTest);
		RUN_TEST_CASE(toString);
		RUN_TEST_CASE(fromString);
		RUN_TEST_CASE(add);
		RUN_TEST_CASE(subtract);
		RUN_TEST_CASE(multiply);
		END_TEST(DollarsOperationsTest);
	}

private:
	void assertStringMatch(Dollars const& dollars,
	                       std::string const& expectedString) {
		std::stringstream output;
		dollars.printTo(output);
		ASSERT_EQUAL(output.str(), expectedString);
	}
	
	void toString() {
		this->assertStringMatch(Dollars(), "$0.00");
		this->assertStringMatch(Dollars(0, 1), "$0.01");
		this->assertStringMatch(Dollars(0, -1), "($0.01)");
		this->assertStringMatch(Dollars(1), "$1.00");
		this->assertStringMatch(Dollars(-1), "($1.00)");
		this->assertStringMatch(Dollars(1123012001000, 12),
		                        "$1,123,012,001,000.12");
	}
	
	void fromString() {
		ASSERT_EQUAL(Dollars(""), Dollars());
		ASSERT_EQUAL(Dollars("0"), Dollars());
		ASSERT_EQUAL(Dollars("$0"), Dollars());
		ASSERT_EQUAL(Dollars("0.00"), Dollars());
		ASSERT_EQUAL(Dollars("$0.00"), Dollars());
		ASSERT_THROW(Dollars("0.0"), std::runtime_error);
		ASSERT_THROW(Dollars("$0.0"), std::runtime_error);
		ASSERT_THROW(Dollars("0.000"), std::runtime_error);
		ASSERT_THROW(Dollars("$0.000"), std::runtime_error);
		
		ASSERT_EQUAL(Dollars("$1"), Dollars(1));
		ASSERT_EQUAL(Dollars("$1.00"), Dollars(1));
		ASSERT_EQUAL(Dollars("$1.01"), Dollars(1, 1));
		ASSERT_EQUAL(Dollars("$1,123,012,001,000.12"),
		             Dollars(1123012001000, 12));
		
		ASSERT_EQUAL(Dollars("()"), Dollars());
		ASSERT_EQUAL(Dollars("($0)"), Dollars());
		ASSERT_EQUAL(Dollars("($0.00)"), Dollars());
		ASSERT_THROW(Dollars("$(0)"), std::runtime_error);
		ASSERT_THROW(Dollars("($0"), std::runtime_error);
		ASSERT_THROW(Dollars("$0)"), std::runtime_error);
		
		ASSERT_EQUAL(Dollars("($1)"), Dollars(-1));
		ASSERT_EQUAL(Dollars("($1.00)"), Dollars(-1));
		ASSERT_EQUAL(Dollars("($1.01)"), Dollars(-1, -1));
		ASSERT_EQUAL(Dollars("($1,123,012,001,000.12)"),
		             Dollars(-1123012001000, -12));
	}
	
	void testAdd(Dollars d1, Dollars d2, Dollars expected) {
		ASSERT_EQUAL(d1 + d2, expected);
		d1 += d2;
		ASSERT_EQUAL(d1, expected);
	}
	
	void add() {
		for(int8_t a = -1; a <= 1; ++a) {
			for(int8_t b = -1; b <= 1; ++b) {
				this->testAdd(Dollars(a), Dollars(b), Dollars(a + b));
			}
		}
		
		this->testAdd(Dollars(0, 1),  Dollars(1),    Dollars(1, 1));
		this->testAdd(Dollars(1, 1),  Dollars(2, 2), Dollars(3, 3));
		this->testAdd(Dollars(0, 99), Dollars(0, 1), Dollars(1));
	}
	
	void testSubtract(Dollars d1, Dollars d2, Dollars expected) {
		ASSERT_EQUAL(d1 - d2, expected);
		d1 -= d2;
		ASSERT_EQUAL(d1, expected);
	}
	
	void subtract() {
		for(int8_t a = -1; a <= 1; ++a) {
			for(int8_t b = -1; b <= 1; ++b) {
				this->testSubtract(Dollars(a), Dollars(b), Dollars(a - b));
			}
		}
		
		this->testSubtract(Dollars(1, 1), Dollars(1),    Dollars(0, 1));
		this->testSubtract(Dollars(1, 1), Dollars(0, 1), Dollars(1));
		this->testSubtract(Dollars(3, 3), Dollars(2, 2), Dollars(1, 1));
		this->testSubtract(Dollars(1),    Dollars(0, 1), Dollars(0, 99));
	}
	
	void testMultiply(Dollars d1, double factor, Dollars expected) {
		ASSERT_EQUAL(d1 * factor, expected);
		d1 *= factor;
		ASSERT_EQUAL(d1, expected);
	}
	
	void multiply() {
		this->testMultiply(Dollars(),   10, Dollars());
		this->testMultiply(Dollars(10), 0,  Dollars());
		
		this->testMultiply(Dollars(10),     1,   Dollars(10));
		this->testMultiply(Dollars(10),     2,   Dollars(20));
		this->testMultiply(Dollars(12, 34), 2,   Dollars(24, 68));
		this->testMultiply(Dollars(12, 34), 4,   Dollars(49, 36));
		this->testMultiply(Dollars(12, 34), 100, Dollars(1234));
		
		this->testMultiply(Dollars(12, 34),   -1, Dollars(-12, -34));
		this->testMultiply(Dollars(-12, -34), -1, Dollars(12, 34));
		
		this->testMultiply(Dollars(1), 1.01,  Dollars(1, 1));
		this->testMultiply(Dollars(1), 0.99,  Dollars(0, 99));
		this->testMultiply(Dollars(1), 1.001, Dollars(1));
		this->testMultiply(Dollars(1), 0.999, Dollars(1));
	}

};
