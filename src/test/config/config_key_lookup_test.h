#pragma once

#include <config/config_key.h>
#include <config/config_keys.h>
#include <config/config_key_lookup.h>
#include <test/unit_test.h>

class ConfigKeyLookupTest: public UnitTest {

public:
	bool /* UnitTest:: */ run() override {
		START_TEST(ConfigKeyLookupTest);
		RUN_TEST_CASE(findByFullName);
		RUN_TEST_CASE(findByShortName);
		RUN_TEST_CASE(findById);
		RUN_TEST_CASE(findMissingKey);
		RUN_TEST_CASE(sentinelNotRegistered);
		END_TEST(ConfigKeyLookupTest);
	}

private:
	void findByFullName() {
		ConfigKeyLookup lookup(ConfigKeyLookupTest::testKeys);
		ConfigKey const& testKey = ConfigKeyLookupTest::testKeys[0];
		ConfigKey const& foundKey = lookup.getByFullName(testKey.fullKey);
		assertKeysMatch(foundKey, testKey);
	}
	
	void findByShortName() {
		ConfigKeyLookup lookup(ConfigKeyLookupTest::testKeys);
		ConfigKey const& testKey = ConfigKeyLookupTest::testKeys[1];
		ConfigKey const& foundKey = lookup.getByShortName(testKey.commandLineKey);
		assertKeysMatch(foundKey, testKey);
	}
	
	void findById() {
		ConfigKeyLookup lookup(ConfigKeyLookupTest::testKeys);
		ConfigKey const& testKey = ConfigKeyLookupTest::testKeys[0];
		ConfigKey const& foundKey = lookup.getById(testKey.id);
		assertKeysMatch(foundKey, testKey);
	}
	
	void assertKeysMatch(ConfigKey const& key1, ConfigKey const& key2) {
		ASSERT_EQUAL(key1.id, key2.id);
		ASSERT_TRUE(key1.type == key2.type);
		ASSERT_EQUAL(key1.fullKey, key2.fullKey);
		ASSERT_EQUAL(key1.commandLineKey, key2.commandLineKey);
		ASSERT_EQUAL(key1.description, key2.description);
		ASSERT_EQUAL(key1.defaultValue, key2.defaultValue);
		ASSERT_EQUAL(key1.isCommandLineArgument, key2.isCommandLineArgument);
	}
	
	void findMissingKey() {
		ConfigKeyLookup lookup(ConfigKeyLookupTest::testKeys);
		ASSERT_ERROR((void)lookup.getByFullName("MissingKey"), ConfigKeyLookup::KeyNotFoundError);
		ASSERT_ERROR((void)lookup.getByShortName("m"), ConfigKeyLookup::KeyNotFoundError);
		ASSERT_ERROR((void)lookup.getByShortName(""), ConfigKeyLookup::KeyNotFoundError);
		ASSERT_ERROR((void)lookup.getById(ConfigKeys::ShowHelp), ConfigKeyLookup::KeyNotFoundError);
	}
	
	void sentinelNotRegistered() {
		ConfigKeyLookup lookup(ConfigKeyLookupTest::testKeys);
		ASSERT_ERROR((void)lookup.getByFullName(""), ConfigKeyLookup::KeyNotFoundError);
		ASSERT_ERROR((void)lookup.getByShortName(""), ConfigKeyLookup::KeyNotFoundError);
	}
	
	static ConfigKey const testKeys[];

};

ConfigKey const ConfigKeyLookupTest::testKeys[] = {
	// (id, type, full name, short name, description, default value)
	ConfigKey(ConfigKeys::TestKey1, ConfigType::Integer, "FullName1", "", "Description", "1"),
	ConfigKey(ConfigKeys::TestKey2, ConfigType::FloatingPoint, "FullName2", "f", "Description", "2.0"),
	ConfigKey(ConfigKeys::Null, ConfigType::Null, "", "", "", "")
};
