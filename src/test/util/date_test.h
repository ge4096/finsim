#pragma once

#include <test/unit_test.h>
#include <util/date.h>
#include <sstream>
#include <stdexcept>

class DateTest: public UnitTest {

public:
	bool /* UnitTest:: */ run() override {
		START_TEST(DateTest);
		RUN_TEST_CASE(invalidDates);
		RUN_TEST_CASE(comparisons);
		RUN_TEST_CASE(toString);
		RUN_TEST_CASE(dayOfWeek);
		RUN_TEST_CASE(lastDayOfMonth);
		RUN_TEST_CASE(lastDayOfYear);
		RUN_TEST_CASE(incrementOne);
		RUN_TEST_CASE(incrementMany);
		END_TEST(DateTest);
	}

private:
	void invalidDates() {
		ASSERT_ERROR(Date(1970, 0, 0), std::out_of_range);
		ASSERT_ERROR(Date(1970, 0, 1), std::out_of_range);
		ASSERT_ERROR(Date(1970, 1, 0), std::out_of_range);
		ASSERT_ERROR(Date(1970, 1, 32), std::out_of_range);
		ASSERT_ERROR(Date(1970, 2, 29), std::out_of_range);
		ASSERT_ERROR(Date(1970, 13, 1), std::out_of_range);
	}
	
	void comparisons() {
		Date day1(1970, 1, 1);
		Date day2(1970, 1, 2);
		Date day3(1970, 1, 3);
		
		ASSERT_TRUE(day1 == day1);
		ASSERT_FALSE(day1 == day2);
		
		ASSERT_FALSE(day1 != day1);
		ASSERT_TRUE(day1 != day2);
		
		ASSERT_TRUE(day1 < day2);
		ASSERT_FALSE(day2 < day2);
		ASSERT_FALSE(day3 < day2);
		
		ASSERT_TRUE(day1 <= day2);
		ASSERT_TRUE(day2 <= day2);
		ASSERT_FALSE(day3 <= day2);
		
		ASSERT_FALSE(day1 > day2);
		ASSERT_FALSE(day2 > day2);
		ASSERT_TRUE(day3 > day2);
		
		ASSERT_FALSE(day1 >= day2);
		ASSERT_TRUE(day2 >= day2);
		ASSERT_TRUE(day3 >= day2);
	}
	
	void assertStringMatch(Date const& date, std::string const& expectedString) {
		std::stringstream output;
		date.printTo(output);
		ASSERT_EQUAL(output.str(), expectedString);
	}
	
	void toString() {
		assertStringMatch(Date(1970, 1, 1), "1970-01-01");
		assertStringMatch(Date(1970, 12, 31), "1970-12-31");
	}
	
	void dayOfWeek() {
		ASSERT_EQUAL(Date(1970, 1, 1).getDayOfWeek(), DayOfWeek::Thursday);
		ASSERT_EQUAL(Date(1970, 1, 2).getDayOfWeek(), DayOfWeek::Friday);
		ASSERT_EQUAL(Date(1970, 12, 31).getDayOfWeek(), DayOfWeek::Thursday);
		ASSERT_EQUAL(Date(2100, 1, 1).getDayOfWeek(), DayOfWeek::Friday);
	}
	
	void lastDayOfMonth() {
		ASSERT_FALSE(Date(1970, 1, 1).isLastDayOfMonth());
		ASSERT_TRUE(Date(1970, 1, 31).isLastDayOfMonth());
		ASSERT_TRUE(Date(1970, 2, 28).isLastDayOfMonth());
		ASSERT_FALSE(Date(1972, 2, 28).isLastDayOfMonth());
		ASSERT_TRUE(Date(1972, 2, 29).isLastDayOfMonth());
		ASSERT_TRUE(Date(1970, 12, 31).isLastDayOfMonth());
	}
	
	void lastDayOfYear() {
		ASSERT_FALSE(Date(1970, 1, 1).isLastDayOfYear());
		ASSERT_FALSE(Date(1970, 1, 31).isLastDayOfYear());
		ASSERT_FALSE(Date(1970, 12, 30).isLastDayOfYear());
		ASSERT_TRUE(Date(1970, 12, 31).isLastDayOfYear());
	}
	
	void incrementOne() {
		Date date = Date(1970, 1, 1);
		++date;
		ASSERT_EQUAL(date, Date(1970, 1, 2));
		
		date = Date(1970, 1, 31);
		++date;
		ASSERT_EQUAL(date, Date(1970, 2, 1));
		
		date = Date(1970, 12, 31);
		++date;
		ASSERT_EQUAL(date, Date(1971, 1, 1));
	}
	
	void incrementMany() {
		Date date = Date(1970, 1, 1);
		for(uint32_t i = 0; i < 365; ++i) {
			++date;
		}
		ASSERT_EQUAL(date, Date(1971, 1, 1));
		
		date = Date(1972, 1, 1);
		for(uint32_t i = 0; i < 366; ++i) {
			++date;
		}
		ASSERT_EQUAL(date, Date(1973, 1, 1));
	}

};
