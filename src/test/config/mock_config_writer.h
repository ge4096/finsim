#pragma once

#include "config/config_writer.h"
#include <map>

class MockConfigWriter: public ConfigWriter {

public:
	void /* ConfigWriter:: */ writeValue(ConfigKey const& key, std::string const& value) override {
		this->values.emplace(key.id, value);
	}
	
	std::map<ConfigKeys::ConfigId, std::string> values;

};
