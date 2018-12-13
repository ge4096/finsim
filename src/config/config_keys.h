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
		// (id, type, default value, full name, short name, description)
		ConfigKey(ConfigFilename, ConfigType::String, "",
		          "ConfigFilename", "c", "The name of the configuration file"),
		ConfigKey(ShowHelp, ConfigType::Boolean, "false",
		          "ShowHelp", "h", "Shows help"),
		ConfigKey(Null, ConfigType::Null, "", "", "", "")
	};
}
