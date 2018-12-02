#pragma once

#include "config/argument_loader.h"
#include "config/config_keys.h"
#include "config/config_key_lookup.h"
#include "test/config/mock_config_writer.h"
#include "test/unit_test.h"
#include "util/components.h"

class ArgumentLoaderTest: public UnitTest {

public:
	bool /* UnitTest:: */ run() override {
		START_TEST(ArgumentLoaderTest);
		RUN_TEST_CASE(noArguments);
		RUN_TEST_CASE(configFilenameSpecified);
		RUN_TEST_CASE(multipleArguments);
		END_TEST(ArgumentLoaderTest);
	}
	
	void /* UnitTest:: */ setUp() override {
		this->components.configKeyLookup = new ConfigKeyLookup(ArgumentLoaderTest::testKeys);
		this->mockConfigWriter = new MockConfigWriter();
		this->components.configWriter = this->mockConfigWriter;
	}
	
	void /* UnitTest:: */ tearDown() override {
		delete this->mockConfigWriter;
		this->mockConfigWriter = nullptr;
		this->components.configWriter = nullptr;
		delete this->components.configKeyLookup;
		this->components.configKeyLookup = nullptr;
	}

private:
	void noArguments() {
		char const* programName = "program";
		char const* argv[] = {programName};
		constexpr int argc = sizeof(argv) / sizeof(char const*);
		
		ArgumentLoader argumentLoader(this->components, argc, argv);
		auto const& loadedValues = this->mockConfigWriter->values;
		ASSERT_EQUAL(loadedValues.size(), 1);
		ASSERT_EQUAL(loadedValues.at(ConfigKeys::ConfigFilename), "program.cfg");
	}
	
	void configFilenameSpecified() {
		char const* programName = "program";
		char const* configFlag = "-c";
		char const* configFilename = "myfile.cfg";
		char const* argv[] = {programName, configFlag, configFilename};
		constexpr int argc = sizeof(argv) / sizeof(char const*);
		
		ArgumentLoader argumentLoader(this->components, argc, argv);
		auto const& loadedValues = this->mockConfigWriter->values;
		ASSERT_EQUAL(loadedValues.size(), 1);
		ASSERT_EQUAL(loadedValues.at(ConfigKeys::ConfigFilename), configFilename);
	}
	
	void multipleArguments() {
		char const* programName = "program";
		char const* testKey1Flag = "-t";
		char const* testKey1Value = "10";
		char const* testKey2Flag = "--test2";
		char const* testKey2Value = "20.0";
		char const* argv[] = {programName, testKey1Flag, testKey1Value,
		                                   testKey2Flag, testKey2Value};
		constexpr int argc = sizeof(argv) / sizeof(char const*);

		ArgumentLoader argumentLoader(this->components, argc, argv);
		auto const& loadedValues = this->mockConfigWriter->values;
		ASSERT_EQUAL(loadedValues.size(), 3);
		ASSERT_EQUAL(loadedValues.at(ConfigKeys::TestKey1), testKey1Value);
		ASSERT_EQUAL(loadedValues.at(ConfigKeys::TestKey2), testKey2Value);
	}
	
	Components components;
	MockConfigWriter* mockConfigWriter;
	
	static ConfigKey const testKeys[];

};

ConfigKey const ArgumentLoaderTest::testKeys[] = {
	// (id, type, full name, short name, description, default value)
	ConfigKey(ConfigKeys::ConfigFilename, ConfigType::String, "ConfigFilename", "c", "The name of the configuration file", ""),
	ConfigKey(ConfigKeys::TestKey1, ConfigType::Integer, "FullName1", "t", "Description", "1"),
	ConfigKey(ConfigKeys::TestKey2, ConfigType::FloatingPoint, "FullName2", "test2", "Description", "2.0"),
	ConfigKey(ConfigKeys::Null, ConfigType::Null, "", "", "", "")
};
