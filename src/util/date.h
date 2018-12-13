#pragma once

#include "util/day_of_week.h"
#include <cstdint>
#include <ostream>
#include <stdexcept>

class Date {

public:
	Date(uint16_t year, uint8_t month, uint8_t dayOfMonth) {
		this->setYear(year);
		this->setMonth(month);
		this->setDayOfMonth(dayOfMonth);
		this->dayOfWeek = zellersCongruence(year, month, dayOfMonth);
	}
	
	void printTo(std::ostream& outputStream) const {
		outputStream << this->date.date.year;
		
		uint8_t const& month = this->date.date.month;
		if(month < 10) {
			outputStream << "-0" << static_cast<uint32_t>(month);
		}
		else {
			outputStream << "-" << static_cast<uint32_t>(month);
		}
		
		uint8_t const& dayOfMonth = this->date.date.dayOfMonth;
		if(dayOfMonth < 10) {
			outputStream << "-0" << static_cast<uint32_t>(dayOfMonth);
		}
		else {
			outputStream << "-" << static_cast<uint32_t>(dayOfMonth);
		}
	}
	
	uint16_t getYear() const {
		return this->date.date.year;
	}
	
	uint8_t getMonth() const {
		return this->date.date.month;
	}
	
	uint8_t getDayOfMonth() const {
		return this->date.date.dayOfMonth;
	}
	
	bool isLastDayOfMonth() const {
		return this->lastDayOfMonth;
	}
	
	bool isLastDayOfYear() const {
		return this->lastDayOfYear;
	}
	
	DayOfWeek getDayOfWeek() const {
		return this->dayOfWeek;
	}
	
	Date const& operator++() {
		if(!(this->lastDayOfMonth)) {
			setDayOfMonth(this->date.date.dayOfMonth + 1);
		}
		else if(!(this->lastDayOfYear)) {
			setMonth(this->date.date.month + 1);
			setDayOfMonth(1);
		}
		else {
			setYear(this->date.date.year + 1);
			setMonth(1);
			setDayOfMonth(1);
		}
		
		++this->dayOfWeek;
		return *this;
	}
	
	bool operator==(Date const& other) const {
		return (this->date.comparableDate == other.date.comparableDate);
	}
	
	bool operator!=(Date const& other) const {
		return (this->date.comparableDate != other.date.comparableDate);
	}
	
	bool operator<(Date const& other) const {
		return (this->date.comparableDate < other.date.comparableDate);
	}
	
	bool operator<=(Date const& other) const {
		return (this->date.comparableDate <= other.date.comparableDate);
	}
	
	bool operator>(Date const& other) const {
		return (this->date.comparableDate > other.date.comparableDate);
	}
	
	bool operator>=(Date const& other) const {
		return (this->date.comparableDate >= other.date.comparableDate);
	}

private:
	void setYear(uint16_t year) {
		this->date.date.year = year;
		
		if((year % 4) != 0) {
			this->leapYear = false;
		}
		else if((year % 100) != 0) {
			this->leapYear = true;
		}
		else if((year % 400) != 0) {
			this->leapYear = false;
		}
		else {
			this->leapYear = true;
		}
	}
	
	void setMonth(uint8_t month) {
		if((month == 0) || (month > Date::monthsInYear)) {
			throw std::out_of_range("Month does not exist");
		}
		
		this->date.date.month = month;
	}
	
	void setDayOfMonth(uint8_t dayOfMonth) {
		uint8_t daysInMonth;
		if(this->leapYear) {
			daysInMonth = Date::leapMonthLengths[this->date.date.month - 1];
		}
		else {
			daysInMonth = Date::nonleapMonthLengths[this->date.date.month - 1];
		}
		
		if((dayOfMonth == 0) || (dayOfMonth > daysInMonth)) {
			throw std::out_of_range("Day does not exist");
		}
		
		this->date.date.dayOfMonth = dayOfMonth;
		
		if(dayOfMonth == daysInMonth) {
			this->lastDayOfMonth = true;
			if(this->date.date.month == Date::monthsInYear) {
				this->lastDayOfYear = true;
			}
			else {
				this->lastDayOfYear = false;
			}
		}
		else {
			this->lastDayOfMonth = false;
			this->lastDayOfYear = false;
		}
	}
	
	union EfficientDate {
		struct __attribute__((packed, aligned(4))) {
			uint8_t dayOfMonth;
			uint8_t month;
			uint16_t year;
		} date;
		uint32_t comparableDate;
	} date;
	
	bool leapYear;
	bool lastDayOfMonth;
	bool lastDayOfYear;
	DayOfWeek dayOfWeek;
	
	static constexpr uint8_t monthsInYear = 12;
	static uint8_t const nonleapMonthLengths[Date::monthsInYear];
	static uint8_t const leapMonthLengths[Date::monthsInYear];

};

uint8_t const Date::nonleapMonthLengths[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

uint8_t const Date::leapMonthLengths[] = {
	31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

std::ostream& operator<<(std::ostream& outputStream, Date const& date) {
	date.printTo(outputStream);
	return outputStream;
}
