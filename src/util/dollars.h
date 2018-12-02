#pragma once

#include <cstdint>
#include <ostream>
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
	
	Dollars operator+(Dollars const& credit) {
		return Dollars(0, this->cents + credit.cents);
	}
	
	Dollars operator-(Dollars const& debit) {
		return Dollars(0, this->cents - debit.cents);
	}
	
	Dollars operator*(double factor) {
		double newCents = this->cents * factor;
		double roundingCorrection = 0.499;
		if(newCents < 0) {
			roundingCorrection *= -1;
		}
		return Dollars(0, newCents + roundingCorrection);
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
