#pragma once

#include "config/config_key.h"
#include "config/config_value_factory.h"
#include "test/unit_test.h"

class ConfigValueFactoryTest: public UnitTest {

public:
	bool /* UnitTest:: */ run() override {
		START_TEST(ConfigValueFactoryTest);
		RUN_TEST_CASE(checkTypes);
		RUN_TEST_CASE(parseIntegers);
		RUN_TEST_CASE(parseUnsignedIntegers);
		RUN_TEST_CASE(parseFloatingPoints);
		RUN_TEST_CASE(parseBooleans);
		RUN_TEST_CASE(parseStrings);
		END_TEST(ConfigValueFactoryTest);
	}

private:
	void checkTypes() {
		ASSERT_EQUAL(sizeof(int), 4);
		ASSERT_EQUAL(sizeof(unsigned int), 4);
		ASSERT_TRUE(sizeof(long long) > sizeof(int));
	}
	
	void parseIntegers() {
		ASSERT_EQUAL(getIntegerValue("0"), 0);
		ASSERT_EQUAL(getIntegerValue("1"), 1);
		ASSERT_EQUAL(getIntegerValue("-1"), -1);
		ASSERT_EQUAL(getIntegerValue("2147483647"),
		                              2147483647);
		ASSERT_EQUAL(getIntegerValue("-2147483648"),
		                              -2147483648);
		ASSERT_ERROR(getIntegerValue("2147483648"), std::runtime_error);
		ASSERT_ERROR(getIntegerValue("-2147483649"), std::runtime_error);
	}
	
	void parseUnsignedIntegers() {
		ASSERT_EQUAL(getUnsignedIntegerValue("0"), 0);
		ASSERT_EQUAL(getUnsignedIntegerValue("1"), 1);
		ASSERT_ERROR(getUnsignedIntegerValue("-1"), std::runtime_error);
		ASSERT_EQUAL(getUnsignedIntegerValue("2147483647"),
		                                      2147483647);
		ASSERT_ERROR(getUnsignedIntegerValue("-2147483648"), std::runtime_error);
		ASSERT_EQUAL(getUnsignedIntegerValue("4294967295"),
		                                      4294967295);
		ASSERT_ERROR(getUnsignedIntegerValue("-4294967296"), std::runtime_error);
	}
	
	void parseFloatingPoints() {
		ASSERT_EQUAL(getFloatingPointValue("0.0"), 0.0);
	}
	
	void parseBooleans() {
		ASSERT_EQUAL(getBooleanValue("true"), true);
		ASSERT_EQUAL(getBooleanValue("Y"), true);
		ASSERT_EQUAL(getBooleanValue("f"), false);
		ASSERT_EQUAL(getBooleanValue("No"), false);
		ASSERT_ERROR(getBooleanValue("maybe"), std::runtime_error);
		ASSERT_ERROR(getBooleanValue(""), std::runtime_error);
	}
	
	void parseStrings() {
		ASSERT_EQUAL(getStringValue(""), "");
		ASSERT_EQUAL(getStringValue("test string"),
		                            "test string");
		ASSERT_EQUAL(getStringValue("12.34"),
		                            "12.34");
		ASSERT_EQUAL(getStringValue("\"'!@#$%^&*()\"'"),
		                            "\"'!@#$%^&*()\"'");
	}
	
	int const& getIntegerValue(std::string const& stringValue) {
		return this->getParsedValue<int>(stringValue, ConfigType::Integer);
	}
	
	unsigned int const& getUnsignedIntegerValue(std::string const& stringValue) {
		return this->getParsedValue<unsigned int>(stringValue, ConfigType::UnsignedInteger);
	}
	
	double const& getFloatingPointValue(std::string const& stringValue) {
		return this->getParsedValue<double>(stringValue, ConfigType::FloatingPoint);
	}
	
	bool const& getBooleanValue(std::string const& stringValue) {
		return this->getParsedValue<bool>(stringValue, ConfigType::Boolean);
	}
	
	std::string const& getStringValue(std::string const& stringValue) {
		return this->getParsedValue<std::string>(stringValue, ConfigType::String);
	}
	
	template<typename T> T const& getParsedValue(std::string const& stringValue, ConfigType type) {
		void const* parsedValue = this->valueFactory.parseValue(stringValue, type);
		return *(static_cast<T const*>(parsedValue));
	}
	
	ConfigValueFactory valueFactory;

};
