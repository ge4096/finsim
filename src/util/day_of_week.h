#pragma once

#include <cstdint>

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
		case DayOfWeek::Monday:    outputStream << "Monday";    break;
		case DayOfWeek::Tuesday:   outputStream << "Tuesday";   break;
		case DayOfWeek::Wednesday: outputStream << "Wednesday"; break;
		case DayOfWeek::Thursday:  outputStream << "Thursday";  break;
		case DayOfWeek::Friday:    outputStream << "Friday";    break;
		case DayOfWeek::Saturday:  outputStream << "Saturday";  break;
		case DayOfWeek::Sunday:    outputStream << "Sunday";    break;
		default: throw std::out_of_range("Invalid day of week");
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
