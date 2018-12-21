#pragma once

#include "test/unit_test.h"
#include "util/dollars.h"
#include <functional>

class DollarsComparisonsTest: public UnitTest {

public:
	bool /* UnitTest:: */ run() override {
		START_TEST(DollarsComparisonsTest);
		RUN_TEST_CASE(equals);
		RUN_TEST_CASE(notEquals);
		RUN_TEST_CASE(lessThan);
		RUN_TEST_CASE(lessThanOrEqual);
		RUN_TEST_CASE(greaterThan);
		RUN_TEST_CASE(greaterThanOrEqual);
		END_TEST(DollarsComparisonsTest);
	}

private:
	typedef std::function<bool(Dollars const&, Dollars const&)> Comparator;
	
	void runComparisonExpectations(bool const* expectations,
	                               Comparator const& comparator) {
		uint8_t expectationIndex = 0;
		for(int8_t dollars = 1; dollars <= 3; ++dollars) {
			for(int8_t cents = 1; cents <= 3; ++cents) {
				bool positiveResult = comparator(Dollars(dollars, cents),
				                                 Dollars(2, 2));
				ASSERT_EQUAL(positiveResult, expectations[expectationIndex]);
				
				Dollars negativeDollars(dollars - 4, cents - 4);
				bool negativeResult = comparator(negativeDollars,
				                                 Dollars(-2, -2));
				ASSERT_EQUAL(negativeResult, expectations[expectationIndex]);
				
				++expectationIndex;
			}
		}
	}
	
	void equals() {
		                          // c1<c2  c1=c2  c1>c2
		const bool expectations[] = {false, false, false,  // d1<d2
		                             false, true,  false,  // d1=d2
		                             false, false, false}; // d1>d2
		this->runComparisonExpectations(expectations,
		                                std::equal_to<Dollars>());
	}
	
	void notEquals() {
		                          // c1<c2  c1=c2  c1>c2
		const bool expectations[] = {true,  true,  true,  // d1<d2
		                             true,  false, true,  // d1=d2
		                             true,  true,  true}; // d1>d2
		this->runComparisonExpectations(expectations,
		                                std::not_equal_to<Dollars>());
	}
	
	void lessThan() {
		                          // c1<c2  c1=c2  c1>c2
		const bool expectations[] = {true,  true,  true,   // d1<d2
		                             true,  false, false,  // d1=d2
		                             false, false, false}; // d1>d2
		this->runComparisonExpectations(expectations,
		                                std::less<Dollars>());
	}
	
	void lessThanOrEqual() {
		                          // c1<c2  c1=c2  c1>c2
		const bool expectations[] = {true,  true,  true,   // d1<d2
		                             true,  true,  false,  // d1=d2
		                             false, false, false}; // d1>d2
		this->runComparisonExpectations(expectations,
		                                std::less_equal<Dollars>());
	}
	
	void greaterThan() {
		                          // c1<c2  c1=c2  c1>c2
		const bool expectations[] = {false, false, false, // d1<d2
		                             false, false, true,  // d1=d2
		                             true,  true,  true}; // d1>d2
		this->runComparisonExpectations(expectations,
		                                std::greater<Dollars>());
	}
	
	void greaterThanOrEqual() {
		                          // c1<c2  c1=c2  c1>c2
		const bool expectations[] = {false, false, false, // d1<d2
		                             false, true,  true,  // d1=d2
		                             true,  true,  true}; // d1>d2
		this->runComparisonExpectations(expectations,
		                                std::greater_equal<Dollars>());
	}

};
