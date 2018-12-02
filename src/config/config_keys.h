#pragma once

#include "config/config_key.h"

namespace ConfigKeys {
	enum ConfigId: HashableConfigId {
		ConfigFilename,
		ShowHelp,
		TestKey1,
		TestKey2,
		Null
	};

	static ConfigKey const keys[] = {
		// (id, type, full name, short name, description, default value)
		ConfigKey(ConfigFilename, ConfigType::String, "ConfigFilename", "c", "The name of the configuration file", ""),
		ConfigKey(ShowHelp, ConfigType::Boolean, "ShowHelp", "h", "Shows help", "false"),\
		ConfigKey(Null, ConfigType::Null, "", "", "", "")
	};
}
