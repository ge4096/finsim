#pragma once

#include "test/unit_test.h"
#include "util/dollars.h"
#include <functional>
#include <sstream>

class DollarsTest: public UnitTest {

public:
	bool /* UnitTest:: */ run() override {
		START_TEST(DollarsTest);
		RUN_TEST_CASE(equals);
		RUN_TEST_CASE(notEquals);
		RUN_TEST_CASE(lessThan);
		RUN_TEST_CASE(lessThanOrEqual);
		RUN_TEST_CASE(greaterThan);
		RUN_TEST_CASE(greaterThanOrEqual);
		RUN_TEST_CASE(toString);
		RUN_TEST_CASE(add);
		RUN_TEST_CASE(subtract);
		RUN_TEST_CASE(multiply);
		END_TEST(DollarsTest);
	}

private:
	void runComparisonExpectations(bool const* expectations, std::function<bool(Dollars const&, Dollars const&)> const& comparator) {
		uint8_t expectationIndex = 0;
		for(int8_t dollars = 1; dollars <= 3; ++dollars) {
			for(int8_t cents = 1; cents <= 3; ++cents) {
				if(expectations[expectationIndex] == true) {
					ASSERT_TRUE(comparator(Dollars(dollars, cents), Dollars(2, 2)));
				}
				else {
					ASSERT_FALSE(comparator(Dollars(dollars, cents), Dollars(2, 2)));
				}
				++expectationIndex;
			}
		}
		
		expectationIndex = 0;
		for(int8_t dollars = -3; dollars <= -1; ++dollars) {
			for(int8_t cents = -3; cents <= -1; ++cents) {
				if(expectations[expectationIndex] == true) {
					ASSERT_TRUE(comparator(Dollars(dollars, cents), Dollars(-2, -2)));
				}
				else {
					ASSERT_FALSE(comparator(Dollars(dollars, cents), Dollars(-2, -2)));
				}
				++expectationIndex;
			}
		}
	}
	
	void equals() {
		                          // c1<c2  c1=c2  c1>c2
		const bool expectations[] = {false, false, false,  // d1<d2
		                             false, true,  false,  // d1=d2
		                             false, false, false}; // d1>d2
		this->runComparisonExpectations(expectations, std::equal_to<Dollars>());
	}
	
	void notEquals() {
		                          // c1<c2  c1=c2  c1>c2
		const bool expectations[] = {true,  true,  true,  // d1<d2
		                             true,  false, true,  // d1=d2
		                             true,  true,  true}; // d1>d2
		this->runComparisonExpectations(expectations, std::not_equal_to<Dollars>());
	}
	
	void lessThan() {
		                          // c1<c2  c1=c2  c1>c2
		const bool expectations[] = {true,  true,  true,   // d1<d2
		                             true,  false, false,  // d1=d2
		                             false, false, false}; // d1>d2
		this->runComparisonExpectations(expectations, std::less<Dollars>());
	}
	
	void lessThanOrEqual() {
		                          // c1<c2  c1=c2  c1>c2
		const bool expectations[] = {true,  true,  true,   // d1<d2
		                             true,  true,  false,  // d1=d2
		                             false, false, false}; // d1>d2
		this->runComparisonExpectations(expectations, std::less_equal<Dollars>());
	}
	
	void greaterThan() {
		                          // c1<c2  c1=c2  c1>c2
		const bool expectations[] = {false, false, false, // d1<d2
		                             false, false, true,  // d1=d2
		                             true,  true,  true}; // d1>d2
		this->runComparisonExpectations(expectations, std::greater<Dollars>());
	}
	
	void greaterThanOrEqual() {
		                          // c1<c2  c1=c2  c1>c2
		const bool expectations[] = {false, false, false, // d1<d2
		                             false, true,  true,  // d1=d2
		                             true,  true,  true}; // d1>d2
		this->runComparisonExpectations(expectations, std::greater_equal<Dollars>());
	}
	
	void assertStringMatch(Dollars const& dollars, std::string const& expectedString) {
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
		this->assertStringMatch(Dollars(1123012001000, 12), "$1,123,012,001,000.12");
	}
	
	void testAdd(Dollars d1, Dollars d2, Dollars expected) {
		ASSERT_EQUAL(d1 + d2, expected);
		d1 += d2;
		ASSERT_EQUAL(d1, expected);
	}
	
	void add() {
		this->testAdd(Dollars(),   Dollars(),   Dollars());
		this->testAdd(Dollars(1),  Dollars(),   Dollars(1));
		this->testAdd(Dollars(),   Dollars(1),  Dollars(1));
		this->testAdd(Dollars(1),  Dollars(1),  Dollars(2));
		this->testAdd(Dollars(-1), Dollars(),   Dollars(-1));
		this->testAdd(Dollars(),   Dollars(-1), Dollars(-1));
		this->testAdd(Dollars(-1), Dollars(-1), Dollars(-2));
		this->testAdd(Dollars(1),  Dollars(-1), Dollars());
		this->testAdd(Dollars(-1), Dollars(1),  Dollars());
		
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
		this->testSubtract(Dollars(),   Dollars(),   Dollars());
		this->testSubtract(Dollars(1),  Dollars(),   Dollars(1));
		this->testSubtract(Dollars(),   Dollars(1),  Dollars(-1));
		this->testSubtract(Dollars(1),  Dollars(1),  Dollars());
		this->testSubtract(Dollars(-1), Dollars(),   Dollars(-1));
		this->testSubtract(Dollars(),   Dollars(-1), Dollars(1));
		this->testSubtract(Dollars(-1), Dollars(-1), Dollars());
		this->testSubtract(Dollars(1),  Dollars(-1), Dollars(2));
		this->testSubtract(Dollars(-1), Dollars(1),  Dollars(-2));
		
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
		this->testMultiply(Dollars(12, 34), 100, Dollars(1234));
		
		this->testMultiply(Dollars(12, 34),   -1, Dollars(-12, -34));
		this->testMultiply(Dollars(-12, -34), -1, Dollars(12, 34));
		
		this->testMultiply(Dollars(1), 1.01,  Dollars(1, 1));
		this->testMultiply(Dollars(1), 0.99,  Dollars(0, 99));
		this->testMultiply(Dollars(1), 1.001, Dollars(1));
		this->testMultiply(Dollars(1), 0.999, Dollars(1));
	}

};
