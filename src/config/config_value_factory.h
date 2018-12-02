#pragma once

#include <config/config_key.h>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

bool charToBool(char firstLetter) {
	switch(firstLetter) {
		case 't':
		case 'T':
		case 'y':
		case 'Y':
			return true;
		case 'f':
		case 'F':
		case 'n':
		case 'N':
			return false;
		default:
			throw std::runtime_error("Unrecognized boolean value");
	}
}

class ConfigValueFactory {

public:
	ConfigValueFactory() {}
	
	~ConfigValueFactory() {
		this->deleteAllParsedValues();
	}
	
	void const* parseValue(std::string const& stringValue, ConfigType type) {
		switch(type) {
			case ConfigType::Integer:
				return this->parseInteger(stringValue);
			case ConfigType::UnsignedInteger:
				return this->parseUnsignedInteger(stringValue);
			case ConfigType::FloatingPoint:
				return this->parseFloatingPoint(stringValue);
			case ConfigType::Boolean:
				return this->parseBoolean(stringValue);
			case ConfigType::String:
				return this->parseString(stringValue);
			default:
				throw std::runtime_error("Invalid type");
		}
	}

private:
	void const* parseInteger(std::string const& stringValue) {
		long long parsedValue = std::stoll(stringValue);
		if(parsedValue < std::numeric_limits<int>::min() ||
		   parsedValue > std::numeric_limits<int>::max()) {
			throw std::runtime_error("Integer out of range");
		}
		int const* newValue = new int(parsedValue);
		this->parsedIntegers.push_back(newValue);
		return static_cast<void const*>(newValue);
	}
	
	void const* parseUnsignedInteger(std::string const& stringValue) {
		long long parsedValue = std::stoll(stringValue);
		if(parsedValue < std::numeric_limits<unsigned int>::min() ||
		   parsedValue > std::numeric_limits<unsigned int>::max()) {
			throw std::runtime_error("Unsigned integer out of range");
		}
		unsigned int const* newValue = new unsigned int(parsedValue);
		this->parsedUnsignedIntegers.push_back(newValue);
		return static_cast<void const*>(newValue);
	}
	
	void const* parseFloatingPoint(std::string const& stringValue) {
		double* parsedValue = new double();
		*parsedValue = std::stod(stringValue);
		this->parsedFloatingPoints.push_back(parsedValue);
		return static_cast<void const*>(parsedValue);
	}
	
	void const* parseBoolean(std::string const& stringValue) {
		if(stringValue.empty()) {
			throw std::runtime_error("Cannot parse empty boolean value");
		}
		bool* parsedValue = new bool();
		*parsedValue = charToBool(stringValue[0]);
		this->parsedBooleans.push_back(parsedValue);
		return static_cast<void const*>(parsedValue);
	}
	
	void const* parseString(std::string const& stringValue) {
		std::string const* newString = new std::string(stringValue);
		this->parsedStrings.push_back(newString);
		return static_cast<void const*>(newString);
	}
	
	void deleteAllParsedValues() {
		deleteParsedValues(this->parsedIntegers);
		deleteParsedValues(this->parsedUnsignedIntegers);
		deleteParsedValues(this->parsedFloatingPoints);
		deleteParsedValues(this->parsedBooleans);
		deleteParsedValues(this->parsedStrings);
	}
	
	template<typename T> void deleteParsedValues(std::vector<T const*> const& parsedValues) {
		for(auto& parsedValue: parsedValues) {
			delete parsedValue;
		}
	}
	
	std::vector<int const*> parsedIntegers;
	std::vector<unsigned int const*> parsedUnsignedIntegers;
	std::vector<double const*> parsedFloatingPoints;
	std::vector<bool const*> parsedBooleans;
	std::vector<std::string const*> parsedStrings;

};
