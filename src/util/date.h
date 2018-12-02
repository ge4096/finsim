#pragma once

#include <cstdint>
#include <ostream>
#include <stdexcept>

enum class DayOfWeek: uint8_t {
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday,
	Sunday,
	Null
};

DayOfWeek& operator++(DayOfWeek& dayOfWeek) {
	dayOfWeek = static_cast<DayOfWeek>(static_cast<uint8_t>(dayOfWeek) + 1);
	if(dayOfWeek == DayOfWeek::Null) {
		dayOfWeek = static_cast<DayOfWeek>(0);
	}
	return dayOfWeek;
}

std::ostream& operator<<(std::ostream& outputStream,
                         DayOfWeek const& dayOfWeek) {
	switch(dayOfWeek) {
		case DayOfWeek::Monday:
			outputStream << "Monday";
			break;
		case DayOfWeek::Tuesday:
			outputStream << "Tuesday";
			break;
		case DayOfWeek::Wednesday:
			outputStream << "Wednesday";
			break;
		case DayOfWeek::Thursday:
			outputStream << "Thursday";
			break;
		case DayOfWeek::Friday:
			outputStream << "Friday";
			break;
		case DayOfWeek::Saturday:
			outputStream << "Saturday";
			break;
		case DayOfWeek::Sunday:
			outputStream << "Sunday";
			break;
		default:
			throw std::out_of_range("Invalid day of week");
	}
	
	return outputStream;
}

DayOfWeek zellersCongruence(uint16_t year, uint8_t month, uint8_t day) {
	if(month <= 2) {
		--year;
		month += 12;
	}
	
	int32_t century = year / 100;
	int32_t yearOfCentury = year % 100;
	int32_t result = day;
	result += (13 * (month + 1)) / 5;
	result += yearOfCentury + (yearOfCentury / 4);
	result += (century / 4) + (5 * century);
	return static_cast<DayOfWeek>((result + 5) % 7);
}

class Date {

public:
	Date(uint16_t year, uint8_t month, uint8_t dayOfMonth) {
		this->setYear(year);
		this->setMonth(month);
		this->setDayOfMonth(dayOfMonth);
		this->dayOfWeek = zellersCongruence(year, month, dayOfMonth);
	}
	
	void printTo(std::ostream& outputStream) const {
		outputStream << year;
		
		if(this->month < 10) {
			outputStream << "-0" << static_cast<uint32_t>(this->month);
		}
		else {
			outputStream << "-" << static_cast<uint32_t>(this->month);
		}
		
		if(this->dayOfMonth < 10) {
			outputStream << "-0" << static_cast<uint32_t>(this->dayOfMonth);
		}
		else {
			outputStream << "-" << static_cast<uint32_t>(this->dayOfMonth);
		}
	}
	
	uint16_t getYear() const {
		return this->year;
	}
	
	uint8_t getMonth() const {
		return this->month;
	}
	
	uint8_t getDayOfMonth() const {
		return this->dayOfMonth;
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
			setDayOfMonth(this->dayOfMonth + 1);
		}
		else if(!(this->lastDayOfYear)) {
			setMonth(this->month + 1);
			setDayOfMonth(1);
		}
		else {
			setYear(this->year + 1);
			setMonth(1);
			setDayOfMonth(1);
		}
		
		++this->dayOfWeek;
		return *this;
	}
	
	bool operator==(Date const& other) const {
		return (this->getComparable() == other.getComparable());
	}
	
	bool operator!=(Date const& other) const {
		return (this->getComparable() != other.getComparable());
	}
	
	bool operator<(Date const& other) const {
		return (this->getComparable() < other.getComparable());
	}
	
	bool operator<=(Date const& other) const {
		return (this->getComparable() <= other.getComparable());
	}
	
	bool operator>(Date const& other) const {
		return (this->getComparable() > other.getComparable());
	}
	
	bool operator>=(Date const& other) const {
		return (this->getComparable() >= other.getComparable());
	}

private:
	void setYear(uint16_t year) {
		this->year = year;
		
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
		
		this->month = month;
	}
	
	void setDayOfMonth(uint8_t dayOfMonth) {
		uint8_t daysInMonth;
		if(this->leapYear) {
			daysInMonth = Date::leapMonthLengths[this->month - 1];
		}
		else {
			daysInMonth = Date::nonleapMonthLengths[this->month - 1];
		}
		
		if((dayOfMonth == 0) || (dayOfMonth > daysInMonth)){
			throw std::out_of_range("Day does not exist");
		}
		
		this->dayOfMonth = dayOfMonth;
		
		if(dayOfMonth == daysInMonth) {
			this->lastDayOfMonth = true;
			if(this->month == Date::monthsInYear) {
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
	
	uint32_t getComparable() const {
		return ((this->year << 16) | (this->month << 8) | this->dayOfMonth);
	}
	
	uint16_t year;
	bool leapYear;
	uint8_t month;
	uint8_t dayOfMonth;
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
