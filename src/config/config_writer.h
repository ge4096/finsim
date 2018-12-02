#pragma once

#include "config/config_key.h"
#include <string>

class ConfigWriter {

public:
	virtual ~ConfigWriter() {}
	
	virtual void writeValue(ConfigKey const& key,
	                        std::string const& value) = 0;

};
