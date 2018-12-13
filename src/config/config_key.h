#pragma once

#include <string>

enum class ConfigType {
	Integer,
	UnsignedInteger,
	FloatingPoint,
	Boolean,
	String,
	Null
};

namespace ConfigKeys {
	typedef uint32_t HashableConfigId;
	enum ConfigId: HashableConfigId;
}

class ConfigKey {

public:
	ConfigKey(ConfigKeys::ConfigId id, ConfigType type,
	          char const* defaultValue, char const* fullKey,
	          char const* commandLineKey, char const* description) {
		this->id = id;
		this->type = type;
		this->fullKey = fullKey;
		this->commandLineKey = commandLineKey;
		this->isCommandLineArgument = (commandLineKey[0] != '\0');
		this->description = description;
		this->defaultValue = defaultValue;
	}
	
	std::string fullKey;
	std::string commandLineKey;
	std::string description;
	std::string defaultValue;
	ConfigType type;
	ConfigKeys::ConfigId id;
	bool isCommandLineArgument;

};

namespace std {
	using ConfigKeys::ConfigId;
	using ConfigKeys::HashableConfigId;
	
	template<> struct hash<ConfigId> {
		size_t operator()(ConfigId const& id) const {
			HashableConfigId hashable = static_cast<HashableConfigId>(id);
			return std::hash<HashableConfigId>()(hashable);
		}
	};
}
