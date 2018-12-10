#pragma once

#include <cstdint>
#include <ostream>
#include <string>
#include <vector>

class Dollars {

public:
	Dollars() {
		this->cents = 0;
	}
	
	Dollars(int64_t dollars) {
		this->cents = dollars * 100;
	}
	
	Dollars(int64_t dollars, int64_t cents) {
		if(((dollars < 0) && (cents > 0)) ||
		   ((dollars > 0) && (cents < 0))) {
			throw std::runtime_error("Dollar and cent signs do not match");
		}
		
		this->cents = (dollars * 100) + cents;
	}
	
	Dollars(std::string const& fromString) {
		this->cents = 0;
		bool decimalFound = false;
		bool isNegative = false;
		size_t parseStartIndex = 0;
		size_t parseEndIndex = fromString.length();
		if((fromString.front() == '(') && (fromString.back() == ')')) {
			isNegative = true;
			parseStartIndex += 1;
			parseEndIndex -= 1;
		}
		
		for(size_t i = parseStartIndex; i < parseEndIndex; ++i) {
			if((fromString[i] >= '0') && (fromString[i] <= '9')) {
				this->cents = (this->cents * 10) + (fromString[i] - '0');
			}
			else if(fromString[i] == '.') {
				decimalFound = true;
				if(i != (parseEndIndex - 3)) {
					throw std::runtime_error("Misplaced decimal point parsed");
				}
			}
			else if(fromString[i] == '$') {
				if(i != parseStartIndex) {
					throw std::runtime_error("Unexpected character parsed");
				}
			}
			else if(fromString[i] == ',') {
				if(decimalFound) {
					throw std::runtime_error("Unexpected character parsed");
				}
			}
			else {
				throw std::runtime_error("Unexpected character parsed");
			}
		}
		
		if(!decimalFound) {
			this->cents *= 100;
		}
		if(isNegative) {
			this->cents *= -1;
		}
	}
	
	void printTo(std::ostream& outputStream) const {
		int64_t normalizedCents = this->cents;
		if(this->cents < 0) {
			normalizedCents *= -1;
			outputStream << "(";
		}
		
		uint64_t normalizedDollarPart = normalizedCents / 100;
		std::vector<uint16_t> dollarsDigitGroups;
		uint64_t dollarsDigitGroupValue = normalizedDollarPart;
		do {
			dollarsDigitGroups.push_back(dollarsDigitGroupValue % 1000);
			dollarsDigitGroupValue /= 1000;
		} while(dollarsDigitGroupValue > 0);
		outputStream << "$" << dollarsDigitGroups.back();
		dollarsDigitGroups.pop_back();
		while(dollarsDigitGroups.size() > 0) {
			uint16_t const& dollarsDigitGroup = dollarsDigitGroups.back();
			if(dollarsDigitGroup < 10) {
				outputStream << ",00" << dollarsDigitGroup;
			}
			else if(dollarsDigitGroup < 100) {
				outputStream << ",0" << dollarsDigitGroup;
			}
			else {
				outputStream << "," << dollarsDigitGroup;
			}
			dollarsDigitGroups.pop_back();
		}
		
		uint32_t normalizedCentPart = normalizedCents % 100;
		if(normalizedCentPart < 10) {
			outputStream << ".0" << normalizedCentPart;
		}
		else {
			outputStream << "." << normalizedCentPart;
		}
		
		if(this->cents < 0) {
			outputStream << ")";
		}
	}
	
	Dollars operator+(Dollars const& credit) const {
		return Dollars(0, this->cents + credit.cents);
	}
	
	Dollars operator+=(Dollars const& credit) {
		this->cents += credit.cents;
		return *this;
	}
	
	Dollars operator-(Dollars const& debit) const {
		return Dollars(0, this->cents - debit.cents);
	}
	
	Dollars operator-=(Dollars const& debit) {
		this->cents -= debit.cents;
		return *this;
	}
	
	Dollars operator*(double factor) const {
		double newCents = this->cents * factor;
		double roundingCorrection = 0.499;
		if(newCents < 0) {
			roundingCorrection *= -1;
		}
		return Dollars(0, newCents + roundingCorrection);
	}
	
	Dollars operator*=(double factor) {
		*this = (*this * factor);
		return *this;
	}
	
	bool operator==(Dollars const& other) const {
		return (this->cents == other.cents);
	}
	
	bool operator!=(Dollars const& other) const {
		return (this->cents != other.cents);
	}
	
	bool operator<(Dollars const& other) const {
		return (this->cents < other.cents);
	}
	
	bool operator<=(Dollars const& other) const {
		return (this->cents <= other.cents);
	}
	
	bool operator>(Dollars const& other) const {
		return (this->cents > other.cents);
	}
	
	bool operator>=(Dollars const& other) const {
		return (this->cents >= other.cents);
	}

private:
	int64_t cents;

};

std::ostream& operator<<(std::ostream& outputStream, Dollars const& dollars) {
	dollars.printTo(outputStream);
	return outputStream;
}

Dollars operator*(double const& factor, Dollars const& amount) {
	return (amount * factor);
}
